// $Id$
// Originally from: // $Id: field_ui.js,v 1.6 2010/10/06 13:09:11 dries Exp $

(function($) {

Drupal.behaviors.contentEasementFieldOverview = {
  attach: function (context, settings) {
    $('fieldset#edit-new-field', context).once('edit-new-field', function () {
      Drupal.contentEasementFieldOverview.attachUpdateSelects(this, settings);
    });
    $('fieldset#edit-share-field', context).once('edit-share-field', function () {
      Drupal.contentEasementFieldOverview.attachUpdateSelects(this, settings);
    });
  }
};

Drupal.contentEasementFieldOverview = {
  /**
   * Implements dependent select dropdowns on the 'Manage fields' screen.
   */
  attachUpdateSelects: function(fieldset, settings) {
    var widgetTypes = settings.fieldWidgetTypes;
    var fields = settings.fields;

    // Store the default text of widget selects.
    $('select.widget-type-select', fieldset).each(function () {
      this.initialValue = this.options[0].text;
    });

    // 'Field type' select updates its 'Widget' select.
    $('select.field-type-select', fieldset).each(function () {
      this.targetSelect = $('select.widget-type-select', $(this).parents('div').parents('div').eq(0));

      $(this).bind('change keyup', function () {
        var selectedFieldType = this.options[this.selectedIndex].value;
        var options = (selectedFieldType in widgetTypes ? widgetTypes[selectedFieldType] : []);
        this.targetSelect.contentEasementPopulateOptions(options);
      });

      // Trigger change on initial pageload to get the right widget options
      // when field type comes pre-selected (on failed validation).
      $(this).trigger('change', false);
    });

    // 'Existing field' select updates its 'Widget' select and 'Label' textfield.
    $('select.field-select', fieldset).each(function () {
      this.targetSelect = $('select.widget-type-select', $(this).parents('div').parents('div').eq(0));
      this.targetTextfield = $('input.label-textfield', $(this).parents('div').parents('div').eq(0));

      $(this).bind('change keyup', function (e, updateText) {
        var updateText = (typeof updateText == 'undefined' ? true : updateText);
        var selectedField = this.options[this.selectedIndex].value;
        var selectedFieldType = (selectedField in fields ? fields[selectedField].type : '');
        var selectedFieldWidget = (selectedField in fields ? fields[selectedField].widget : '');
        var options = widgetTypes[selectedFieldType];

        this.targetSelect.contentEasementPopulateOptions(options, selectedFieldWidget);

        if (updateText) {
          $(this.targetTextfield).attr('value', (selectedField in fields ? fields[selectedField].label : ''));
        }
      });

      // Trigger change on initial pageload to get the right widget options
      // and label when field type comes pre-selected (on failed validation).
      $(this).trigger('change', false);
    });
  },
};

/**
 * Populates options in a select input.
 */
jQuery.fn.contentEasementPopulateOptions = function (options, selected) {
  return this.each(function () {
    var disabled = false;
    if (options.length == 0) {
      options = [this.initialValue];
      disabled = true;
    }

    // If possible, keep the same widget selected when changing field type.
    // This is based on textual value, since the internal value might be
    // different (options_buttons vs. node_reference_buttons).
    var previousSelectedText = this.options[this.selectedIndex].text;

    var html = '';
    jQuery.each(options, function (value, text) {
      // Figure out which value should be selected. The 'selected' param
      // takes precedence.
      var is_selected = ((typeof selected != 'undefined' && value == selected) || (typeof selected == 'undefined' && text == previousSelectedText));
      html += '<option value="' + value + '"' + (is_selected ? ' selected="selected"' : '') + '>' + text + '</option>';
    });

    $(this).html(html).attr('disabled', disabled ? 'disabled' : '');
  });
};

})(jQuery);
