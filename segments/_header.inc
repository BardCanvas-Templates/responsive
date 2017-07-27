<?php
/**
 * Template header
 *
 * @package    BardCanvas
 * @subpackage Responsive Template
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

if( $settings->get("templates:responsive.disable_header") == "true" ) return;

$text    = $settings->get("templates:responsive.header_logo_text");
$image   = $settings->get("templates:responsive.header_logo_image");
$bgcolor = $settings->get("templates:responsive.header_background_color");
$links   = $settings->get("templates:responsive.header_links_panel");

if( "{$text}{$image}" == "" ) return;

if( ! empty($links) )
{
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
    
    $links = implode("\n", $items);
}

if( empty($bgcolor) ) $bgcolor = "transparent";
?>

<div id="responsible_custom_header" style="background-color: <?= $bgcolor ?>;">
    <a class="logo" href="<?= $config->full_root_path ?>/">
        <? if( ! empty($image) ): ?>
            <img class="image" src="<?= $image ?>">
        <? endif; ?>
        
        <? if( ! empty($text) ): ?>
            <span class="caption"><?= $text ?></span>
        <? endif; ?>
    </a>
    
    <? if( ! empty($links) ): ?>
        <div class="links_panel">
            <div class="links"><?= $links ?></div>
            <br clear="left">
        </div>
    <? endif; ?>
</div>
