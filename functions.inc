<?php

function add_admin_menu_items_to_header_menu()
{
    global $modules, $current_module, $config, $template, $account;
    
    foreach($modules as $module_name => $this_module)
    {
        if( ! $account->has_admin_rights_to_module($module_name) ) continue;
        
        if(is_array($this_module->menu_items) || is_object($this_module->menu_items))
        {
            foreach($this_module->menu_items->item as $this_menu_item)
            {
                $strict_item_level = trim($this_menu_item["strict_admin_level"]);
                if( ! empty($strict_item_level) &&  $account->level < $strict_item_level ) continue;
                
                $item_url    = trim($this_menu_item->url);
                $added_class = ($module_name == $current_module->name && stristr($_SERVER["PHP_SELF"], "/$module_name/$item_url") !== false) ? "current" : "";
                $url         = "{$config->full_root_path}/$module_name/$item_url";
                $caption     = $this_module->language->admin->menu_items->{$this_menu_item->caption_language_var};
                $icon        = empty($this_module->language->icon) ? "fa-circle-o" : trim($this_module->language->icon);
                
                $html = "
                    <a class='main_menu_item highlighted $added_class' href='$url'>
                        <span class='fa {$icon}'></span> $caption
                    </a>
                ";
                
                $template->add_menu_item(
                    $caption,
                    $html,
                    0
                );
            }
        }
    }
    
}

function build_reponsive_body_style()
{
    global $settings;
    
    $bgcolor  = $settings->get("templates:responsive.body_background_color");
    $bgpos    = $settings->get("templates:responsive.body_background_image_position");
    $bgsize   = $settings->get("templates:responsive.body_background_image_size");
    $bgimage  = $settings->get("templates:responsive.body_background_image");
    $bgrepeat = $settings->get("templates:responsive.body_background_image_repeat");
    
    if( empty($bgcolor)  ) $bgcolor = "white";
    if( empty($bgimage)  ) $bgimage = "none"; else $bgimage = "url('$bgimage')";
    if( empty($bgpos)    ) $bgpos   = "top left";
    if( empty($bgsize)   ) $bgsize  = "contain";
    
    if( empty($bgrepeat) ) $bgrepeat = "repeat";
    $attachment = "";
    if( $bgrepeat == "fixed" )
    {
        $bgrepeat   = "no-repeat";
        $attachment = "background-attachment: fixed;";
    }
    
    return "background: $bgcolor $bgimage $bgpos $bgrepeat; background-size: $bgsize; $attachment";
}

function parse_responsive_links_list($links)
{
    if( empty($links) ) return "";
    
    $items   = array();
    $lines   = explode("\n", $links);
    foreach($lines as $line)
    {
        $line = trim($line);
        if( empty($line) ) continue;
        if( substr($line, 0, 1) != "*" )
        {
            $items[] = $line;
            
            continue;
        }
        
        if( ! preg_match('/\*\s*([\w:@]*)\s*(.*)/', $line, $matches) )
        {
            $items[] = $line;
            
            continue;
        }
        
        $href    = $matches[2];
        $target  = preg_match('/http|https|callto|mailto/i', $href) ? "_blank" : "_self";
        $caption = rtrim($matches[1], ":");
        $caption = substr($caption, 0, 1) != "@" ? $caption
                 : ("<i class='icon fa-stack fa-lg'><i class='fa fa-square fa-stack-2x'></i><i class='fa fa-" . str_replace("@", "", $caption) . " fa-stack-1x fa-inverse'></i></i>");
        
        $items[] = "<a href='{$href}' target='$target'><span>{$caption}</span></a>";
    }
    
    return implode("\n", $items);
}
