#include "game.hxx"

#include "catalog/catalog.hxx"
#include "config/config_manager.hxx"
#include "gfx/renderer.hxx"
#include "input/input.hxx"
#include "main_menu/main_menu_realm.hxx"
#include "platform/platform.hxx"

#include "realm/realm_type_def.hxx"
#include "realm/realm_manager.hxx"
#include "scripts/api/catalog_api.hxx"
#include "scripts/api/config_api.hxx"
#include "scripts/api/log_api.hxx"
#include "scripts/api/realm_api.hxx"
#include "scripts/api/scene_api.hxx"
#include "scripts/api/script_api.hxx"
#include "scripts/api/ui/ui_api.hxx"
#include "scripts/scripting.hxx"
#include "lang/translator.hxx"
#include "lang/translation_loader.hxx"
#include "ui/ui.hxx"
#include "world/world_realm.hxx"

#ifdef NOEDEN_XRAY
#include "xray/xray_manager.hxx"
#include "xray/log_xray.hxx"
#include "xray/scene_xray.hxx"
#include "xray/script_xray.hxx"
#include "xray/ui_xray.hxx"
#include "xray/perf_xray.hxx"
#endif


bool Game::initialize_realms() {
    if (!realms_->add_realm(std::make_unique<MainMenuRealm>())) {
        LOG_ERROR("Failed to add MainMenuRealm");
        return false;
    }
    if (!realms_->add_realm(std::make_unique<WorldRealm>())) {
        LOG_ERROR("Failed to add WorldRealm");
        return false;
    }
    return true;
}

bool Game::initialize() {
    Log::initialize();

    events_ = std::make_unique<EventManager>();
    config_manager_ = std::make_unique<ConfigManager>(events_.get());
    services_ = std::make_unique<ServiceLocator>();
    input_ = std::make_unique<Input>(events_.get());
    scripting_ = std::make_unique<Scripting>(services_.get(), events_.get());
    platform_ = std::make_unique<Platform>(events_.get(), input_.get());
    realms_ = std::make_unique<RealmManager>(services_.get(), events_.get());
    renderer_ = std::make_unique<Renderer>(events_.get());
    ui_ = std::make_unique<Ui>(renderer_.get(), events_.get());
    catalog_ = std::make_unique<Catalog>();

    services_->provide<Ui>(ui_.get());
    services_->provide<Scripting>(scripting_.get());
    services_->provide<RealmManager>(realms_.get());
    services_->provide<Renderer>(renderer_.get());
    services_->provide<Input>(input_.get());
    services_->provide<ConfigManager>(config_manager_.get());
    services_->provide<EventManager>(events_.get());
    services_->provide<Catalog>(catalog_.get());


    if (!platform_->initialize()) {
        return false;
    }
    if (!renderer_->initialize()) {
        return false;
    }
    if (!ui_->initialize()) {
        return false;
    }
    if (!initialize_realms()) {
        return false;
    }

    {
        auto path = fs::absolute(fmt::format("dictionaries/en.toml")).string();
        auto result = TranslationLoader().load(path);
        if (result.error != LoadTranslationError::None) {
            LOG_ERROR("Failed to load translation file: err #{}", (int) result.error);
        }
        t_ = std::make_unique<Translator>(std::move(result.dictionary));
    }
    services_->provide<Translator>(t_.get());

    services_->provide<IInputReader>(input_.get());

    renderer_->set_viewport(platform_->window_size());

    // xray / engine ui
#ifdef NOEDEN_XRAY
    xray_manager_ = std::make_unique<XrayManager>(services_.get(), events_.get());
    xray_manager_->add_xray(std::make_unique<LogXray>());
    xray_manager_->add_xray(std::make_unique<SceneXray>());
    xray_manager_->add_xray(std::make_unique<ScriptXray>());
    xray_manager_->add_xray(std::make_unique<UiXray>());
    xray_manager_->add_xray(std::make_unique<PerfXray>());
#endif

    // scripting
    scripting_->add_api<LogApi>();
    scripting_->add_api<ConfigApi>();
    scripting_->add_api<SceneApi>();
    scripting_->add_api<UiApi>();
    scripting_->add_api<CatalogApi>();
    scripting_->add_api<RealmApi>();
    scripting_->add_api<ScriptApi>();
    scripting_->reload();

    if (!realms_->switch_realm(RealmType::MainMenu)) {
        return false;
    }

    return true;
}

void Game::shutdown() {
#ifdef NOEDEN_XRAY
    xray_manager_.reset();
#endif

    ui_.reset();
    renderer_.reset();

    catalog_.reset();

    scripting_.reset();
    input_.reset();
    config_manager_.reset();

    platform_->shutdown();

    events_.reset();
    services_.reset();
}

Game::~Game() {
    shutdown();
}

int Game::start() {
    Game game;
    game.run();
    return 0;
}

void Game::run() {
    if (!initialize()) {
        LOG_ERROR("Failed to initialize, exiting");
        DEBUG_TRAP();
        return;
    }

    if (!realms_->current_realm()) {
        LOG_ERROR("No initial realm set, exiting");
        DEBUG_TRAP();
        return;
    }

    while (true) {
        Profiler::start_frame();

        if (!platform_->prepare()) {
            break;
        }

        Profiler::timer("Update").start();
        realms_->current_realm()->update();
        Profiler::timer("Update").stop();

        Profiler::timer("Ui").start();
        ui_->update();
        ui_->draw(); // condense into one?
        Profiler::timer("Ui").stop();

        Profiler::timer("Render").start();
        renderer_->render();
        Profiler::timer("Render").stop();

#ifdef NOEDEN_XRAY
        xray_manager_->render();
#endif

        platform_->present();
        Profiler::stop_frame();
    }

    shutdown();
}
