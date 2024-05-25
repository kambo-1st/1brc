<?php
$stations = [];
$fp = fopen('measurements.txt', 'r');
while ($data = fgets($fp, 999)) {
    [$city, $temperature] = explode(';', $data, 2);
    $temp = (float) $temperature;
    if (!isset($stations[$city])) {
        $stations[$city] = [
            $temp,
            $temp,
            $temp,
            1
        ];
    } else {
        $station = &$stations[$city];
        $station[3] ++;
        $station[2] += $temp;
        if ($temp < $station[0]) {
            $station[0] = $temp;
        } else if ($temp > $station[1]) {
            $station[1] = $temp;
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
