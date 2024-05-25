--TEST--
Check if fastfileloader is loaded
--EXTENSIONS--
fastfileloader
--FILE--
<?php
echo 'The extension "fastfileloader" is available';
?>
--EXPECT--
The extension "fastfileloader" is available
