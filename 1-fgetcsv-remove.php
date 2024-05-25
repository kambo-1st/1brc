<?php
$stations = [];
$fp = fopen('measurements.txt', 'r');
while ($data = fgets($fp, null)) {
    $pos = strpos($data, ';');
    $city = substr($data, 0, $pos);
    $temp = substr($data, $pos+1, -1);
    if (!isset($stations[$city])) {
        $stations[$city] = [
            $temp,
            $temp,
            $temp,
            1
        ];
    } else {
        $stations[$city][3] ++;
        $stations[$city][2] += $temp;
        if ($temp < $stations[$city][0]) {
            $stations[$city][0] = $temp;
        }
        if ($temp > $stations[$city][1]) {
            $stations[$city][1] = $temp;
        }
    }
}

ksort($stations);

echo '{';
foreach($stations as $k=>&$station) {
    $station[2] = $station[2]/$station[3];
    echo $k, '=', $station[0], '/', $station[2], '/', $station[1], ", \n";
}
echo '}';
