/**
 * @file
 * Enables the Accordion display on webforms.
 */

(function ($) {
  Drupal.behaviors.webform_accordion = {
    attach: function(context) {
      $('.accordion-container').accordion()
    }
  }
})(jQuery);
