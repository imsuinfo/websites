<?php
/**
 * @file
 * Signaturefield element template file
 */
?>
<label for="<?php echo $element['#id'] ?>-name"><?php echo $element['#title'] ?></label>
<p class='<?php print($element['#id']); ?>-typeItDesc typeItDesc'></p>
<p class='<?php print($element['#id']); ?>-drawItDesc drawItDesc'></p>
<ul class='<?php print($element['#id']); ?>-sigNav sigNav'>
  <li class="<?php print($element['#id']); ?>-typeIt typeIt">
    <a href="#type-it" class="">Type It</a>
  </li>
  <li class="<?php print($element['#id']); ?>-drawIt drawIt">
    <a href="#draw-it" class="">Draw It</a>
  </li>
  <li class="<?php print($element['#id']); ?>-clearButton clearButton">
    <a href='#clear' class="">Clear Signature</a>
  </li>
</ul>
<div class='<?php print($element['#id']); ?>-sig sig sigWrapper'>
  <div class='<?php print($element['#id']); ?>-typed typed'></div>
  <canvas class='pad' width='<?php echo $element['#width'] ?>' height='<?php echo $element['#height'] ?>'></canvas>
  <input type='hidden' id="<?php print($element['#id']); ?>-output" name="<?php print($element['#id']); ?>-output" class='output' value='<?php if(isset($element['#default_value'])) { echo $element['#default_value']; } ?>'>
</div>
<input type="text" id="<?php print($element['#id']); ?>-name" name="<?php print($element['#id']); ?>-name" class="name" size="<?php print($element['#size']); ?>">
<?php if (!empty($element['#description']))  echo "<div class='description'>" . $element['#description'] . "</div>"; ?>
