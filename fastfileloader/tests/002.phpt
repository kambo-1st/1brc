--TEST--
test1() Basic test
--EXTENSIONS--
fastfileloader
--FILE--
<?php
$file = fastfileloader_open(__DIR__.'/'."example2.txt");

while (($line = fastfileloader_readline($file)) !== false) {
    echo $line."\n";
}

fastfileloader_close($file);
?>
--EXPECT--
hello
world