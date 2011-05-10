// $Id$

/**
 *  @file
 *  This will pass the settings and initiate the Msu Slider.
 *  It also sets a maximum height and width for the container dimensions.
 */
(function($) {
Drupal.behaviors.viewsJqfxMsuSlider = {
  attach: function (context) {
    for (id in Drupal.settings.viewsJqfxMsuSlider) {
      $('#' + id + ':not(.viewsJqfxMsuSlider-processed)', context).addClass('viewsJqfxMsuSlider-processed').each(function () {
        var _settings = Drupal.settings.viewsJqfxMsuSlider[$(this).attr('id')];
        var msu = $(this);
        // Fix sizes
        msu.data('hmax', 0).data('wmax', 0);
        $('img', msu).each(function () {
          hmax =  (msu.data('hmax') > $(this).height()) ? msu.data('hmax') : $(this).height();
          wmax =  (msu.data('wmax') > $(this).width()) ? msu.data('hmax') : $(this).width();
          msu.width(wmax).height(hmax).data('hmax', hmax).data('wmax', wmax);
        });
        // Need to pass these settings as functions.
        if (_settings['beforeChange']) {
          var msuBeforeChange = _settings['beforeChange'];
          eval("_settings['beforeChange'] = " + msuBeforeChange);
        }
        if (_settings['afterChange']) {
          var msuAfterChange = _settings['afterChange'];
          eval("_settings['afterChange'] = " + msuAfterChange);
        }
        if (_settings['slideshowEnd']) {
          var msuSlideshowEnd = _settings['slideshowEnd'];
          eval("_settings['slideshowEnd'] = " + msuSlideshowEnd);
        }
        if (_settings['lastSlide']) {
          var msuLastSlide = _settings['lastSlide'];
          eval("_settings['lastSlide'] = " + msuLastSlide);
        }
        if (_settings['afterLoad']) {
          var msuAfterLoad = _settings['afterLoad'];
          eval("_settings['afterLoad'] = " + msuAfterLoad);
        }
        // Load MsuSlider
        $(msu).msuSlider(_settings);
      });
    }
  }
}

})(jQuery);
