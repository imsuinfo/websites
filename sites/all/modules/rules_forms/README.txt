 -------------------------------------------------------------------------
 |||||||||||||||||||||||||||||| RULES FORMS ||||||||||||||||||||||||||||||
 -------------------------------------------------------------------------

 maintained by Jordan Halterman <jordan.halterman@gmail.com>
 http://drupal.org/project/rules_forms

 Thanks to klausi and fago for original development and maintainance of
 the earlier version of Rules Forms, which was included with Rules 1.

 Installation
 ------------
 This module requires Rules 2 (http://drupal.org/project/rules).
 Navigate to administer >> module and enable the Rules Forms module.


 Getting started
 ---------------
 This is a short usage guide to build Rules on you forms:

 * Go to the "Form events" page in the Rules administration menu
   (admin/config/workflow/rules/forms).
 * Select the checkbox "Enable event activation messages on forms" and hit the "Save
   settings" button.
 * Go to the form on your site that you would like to customize with Rules, e.g.
   go to 'node/add/story' to enable events on the "Create Story" form.
 * On the top of the page you see a drupal message with a link to activate events
   for the form, click it.
 * Confirm the activation by clicking the "Activate" button.
 * Go to the "Triggered rules" admin page (admin/rules/trigger) and click the "Add
   a new rule" tab.
 * Fill out the label, choose a form event by selecting one in the "Rules Forms"
   group and confirm with "Save changes".
 * Now you can add conditions and actions to react on the form event.


 Form element conditions and actions
 -----------------------------------

 The Rules forms module allows you to manipulate single form elements, where you
 need the ID of the element. This guide shows you you how to find them.

 * Go to the "Form events" page in the Rules administration menu
   (admin/config/workflow/rules/forms).
 * Make sure that you have activated events on your target form, it should be listed
   on the page.
 * Select the checkbox "Display form element IDs" and hit the "Save settings" button.
 * Go to the target form on your site, where you will see the form element ID below
   each form element.
 * Actions for individual form elements can be taken once that element is loaded
   either by creating or loading an element.
 * To load an element, paste the Element ID in the "Load a form element" action.
 * Once loaded, the data selector may be used to access or alter the element properties.
 * New elements will create data named element_created and loaded elements will create
   data named element_fetched by default.
