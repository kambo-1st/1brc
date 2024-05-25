<?php
$stations = [];
$fp = fopen('measurements.txt', 'r');
while ($data = fgetcsv($fp, null, ';')) {
    if (!isset($stations[$data[0]])) {
        $stations[$data[0]] = [
            $data[1],
            $data[1],
            $data[1],
            1
        ];
    } else {
        $stations[$data[0]][3] ++;
        $stations[$data[0]][2] += $data[1];
        if ($data[1] < $stations[$data[0]][0]) {
            $stations[$data[0]][0] = $data[1];
        }
        if ($data[1] > $stations[$data[0]][1]) {
            $stations[$data[0]][1] = $data[1];
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
