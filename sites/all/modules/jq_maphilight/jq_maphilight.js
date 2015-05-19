(function ($) {
  Drupal.behaviors.jq_maphilight = {
    attach: function(context, settings) {
      var options = {
        fill: (settings.jq_maphilight && settings.jq_maphilight.fill == 'true' ? true : false),
        fillColor: settings.jq_maphilight && settings.jq_maphilight.fillColor ? settings.jq_maphilight.fillColor : '000000',
        fillOpacity: settings.jq_maphilight && settings.jq_maphilight.fillOpacity ? settings.jq_maphilight.fillOpacity : 1 / 10,
        stroke: (settings.jq_maphilight && settings.jq_maphilight.stroke == 'true' ? true : false),
        strokeColor: settings.jq_maphilight && settings.jq_maphilight.strokeColor ? settings.jq_maphilight.strokeColor : 'ff0000',
        strokeOpacity: settings.jq_maphilight && settings.jq_maphilight.strokeOpacity ? settings.jq_maphilight.strokeOpacity : 1,
        strokeWidth: settings.jq_maphilight && settings.jq_maphilight.strokeWidth ? settings.jq_maphilight.strokeWidth : 2,
        fade: (settings.jq_maphilight && settings.jq_maphilight.fade == 'true' ? true : false),
        alwaysOn: (settings.jq_maphilight && settings.jq_maphilight.alwaysOn == 'true' ? true : false),
        neverOn: (settings.jq_maphilight && settings.jq_maphilight.neverOn == 'true' ? true : false),
        groupBy: (settings.jq_maphilight && settings.jq_maphilight.groupBy == 'true' ? true : false)
      }
  if (settings.jq_maphilight && settings.jq_maphilight.allMapsEnabled  == 'true') {
    $('img[usemap]').maphilight(options);
  }
  else {
    $('.jq_maphilight').maphilight(options);
  }
}}})(jQuery);
