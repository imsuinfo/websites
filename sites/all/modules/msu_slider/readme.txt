// $Id$

Views jQFX: Nivo Slider
----------------------------
This module is a Views jQFX addon that integrates the Nivo Slider plugin with views.
The Views jQFX module is a dependency.

Installation
----------------------------
1) If you have not already done so, get and install the Views jQFX module from the drupal project page.
2) Place this module in your modules directory and enable it at admin/modules.
   It will appear in the views section.
3) Download the Nivo Slider plugin and place it in your sites/all/libraries directory.
   To get the plugin to go the project page (http://galleria.aino.se).
   The final directory structure should look like:
   		'sites/all/libraries/nivo-slider/license.txt'
   		'sites/all/libraries/nivo-slider/jquery.nivo.slider.js'
   		'sites/all/libraries/nivo-slider/jquery.nivo.slider.pack.js'
   		'sites/all/libraries/nivo-slider/demo/'
   		etc...
4) Create or edit your content type.
   Captions are created from the title tag attributes of the images in the content type you wish to display.
   If you want to display captions you must have the ability to add this attribute to you image fields.
   To enable image attributes navigate to admin --> structure --> content types
   Select 'manage fields' in the content type(s) that will be displayed in the Nivo Slider (or create a new content type).
   Edit the image field that you want to display and be sure that the 'title' box is checked.
   This will provide a textfield for adding information to uploaded images when creating or editing a node.
   If this is a new content type an image field will need to be created for it.
5) Create the nodes that you wish to display under admin --> content --> add
6) Create a node view. If you are new to views you may want to find a tutorial for it. There are many out there.
   Only the images in the node view will be displayed in the Nivo Slider.
   No additional functionality can be gained by adding fields other than images.
   The module extracts navigation information, captions, and links from the image fields.
   Add the the image fields that you wish to display under the 'fields' section.
   If you want the slide to act as a link, then specify the link information in the image field options of your view.
   Add your filters, arguments, etc for the content.
   Create a new block or page display for your view.
   Blocks can be added to page regions in admin --> structure --> blocks
   Pages must have a path (ie 'nivo-slider').
7) In the settings of your view, click on the link next to the 'Style' label (usually will say 'unformatted').
   Choose the style jQFX and hit the update button.
   This will give you a drop down menu from which to choose your jQFX Settings.
   For the Nivo Slider display, select 'Nivo Slider' under the 'FX Style'.
   The menu will provide your Nivo Slider display options.
   
The Full Nivo Slider Options List .Every option except 'controlNavThumbsFromRel' is available. Thumbnails are generated via search and replace.
-----------------------------------------------------------------------------------------------------------------------------------------------
Name 		 				Type			Default 			Description
effect						String			random	 			Specify sets like: 'fold, fade, sliceDown'
slices						Number			15					Number of slices used in the transition
animSpeed					Number			500 				Slide transition speed in ms
pauseTime					Number			3000				Pause between transitions in ms
startSlide					Number			0 					Set starting Slide (0 index)
directionNav				Boolean			true 				Next & Prev
directionNavHide			Boolean			true 				Only show on hover
controlNav					Boolean			true 				1,2,3...
controlNavThumbs			Boolean			false 				Use thumbnails for Control Nav
controlNavThumbsFromRel		Boolean			false 				Use image rel for thumbs
controlNavThumbsSearch 		String			.jpg 				Replace this with...
controlNavThumbsReplace 	String			_thumb.jpg 			...this in thumb Image src
keyboardNav					Boolean			true 				Use left & right arrows
pauseOnHover				Boolean			true 				Stop animation while hovering
manualAdvance				Boolean			false 				Force manual transitions
captionOpacity				Number			0.8 				Universal caption opacity
beforeChange 				Function		function(){}
afterChange 				Function		function(){}
slideshowEnd 				Function		function(){} 		Triggers after all slides have been shown
lastSlide 					Function		function(){} 		Triggers when last slide is shown
afterLoad 					Function		function(){} 		Triggers when slider has loaded
   
Please post any comments, questions, or bugs in the issues queue of the Views jQFX: Nivo Slider project page on Drupal