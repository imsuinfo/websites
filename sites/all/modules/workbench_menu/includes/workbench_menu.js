(function ($) {
  Drupal.behaviors.workbench_menu_clickable_label = {
    attach:function(context) {
      $('.menu_item-container.clickable.noscript', context).removeClass('noscript').each(function() {
        $(this).children('.menu_item-wrapper').removeClass('noscript');
        $(this).children('.menu_item-wrapper.noscript').children('.menu_item-type-item_label.noscript').removeClass('noscript');

        $(this).children('.menu_item-wrapper').children('.menu_item-type-item_label').click(function() {
          w = $(this).parent();
          c = $(w).parent();

          if (!$(c).hasClass('active') && !$(c).hasClass('active-trail')) {
            if ($(c).hasClass('expanded')) {
              $(c).removeClass('expanded');
              $(c).addClass('collapsed');
              $(w).removeClass('expanded');
              $(w).addClass('collapsed');
              $(this).removeClass('expanded');
              $(this).addClass('collapsed');
            } else if (!$(c).hasClass('active-trail')) {
              $(c).removeClass('collapsed');
              $(c).addClass('expanded');
              $(w).removeClass('collapsed');
              $(w).addClass('expanded');
              $(this).removeClass('collapsed');
              $(this).addClass('expanded');
            }
          }
        });
      });
    }
  }
})(jQuery);
