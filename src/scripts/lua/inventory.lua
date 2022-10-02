local Node = require "ui.lib.node";
local Event = require "event.lib.event";

local inventory_window;
local text;
local sel = 0;

function make_inventory_lines(selected)
    local color_not_selected = "ffffff";
    local color_selected = "00ff00";
    local str = "";

    for i = 1,10,1
    do
        str = str..string.format(
            "$c%s%3i$! %s$n",
            selected == i and color_selected or color_not_selected,
            i,
            "Some Item"
        );
    end

    return str;
end

function on_update_inventory_window(window)
    sel = sel + 1;
    sel = sel % 10;
end

function on_inventory_view(entity_id)
    text:set_text(make_inventory_lines(sel));

    if entity_id == 0 then
        inventory_window:hide();
    else
        inventory_window:set_title(string.format(
            "Inventory of %s",
            scene:entity_name(entity_id)
        ));
        inventory_window:show();
    end
end

function on_load()
    inventory_window = ui:create_window("inventory_window");
    text = ui:create_text("dummy_text")

    if not inventory_window then
        log:error("Could not create inventory window");
        return;
    end

    inventory_window:set_title(string.format("Inventory of %s", scene:player_name() or "?"));
    inventory_window:set_handler(on_update_inventory_window);
    inventory_window:move(0, 0);
    inventory_window:resize(40, 20);
    inventory_window:set_align_x(Node.AlignX.Center);
    inventory_window:set_align_y(Node.AlignY.Center);
    inventory_window:set_anchor_origin(Node.AnchorOrigin.Center);

    text:set_parent(inventory_window);
    text:move(2, 2);
    text:resize(20, 20);
    text:anchor_to(inventory_window);
    text:show();

    events:on(Event.InventoryView, on_inventory_view);
end

function on_unload()
    ui:destroy_node("inventory_window");
end