<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

$command = '../rgrep';

if ('true' === $_GET['insensitive']) {
    $command .= ' -i';
}

$page = max(intval($_GET['page'] ?? 0), 0);
$page *= intval($_GET['size']);

$command .= " -l {$page},{$_GET['size']}";

$command .= " -q \"{$_GET['queryString']}\"";

system($command);

$fp = fopen('result', 'r');

$result = [];

while ($line = fgets($fp)) {
    $temp = explode("\t", trim($line));

    $result[] = [
        'id' => $temp[0],
        'title' => $temp[1],
        'published_at' => $temp[2],
        'content' => $temp[3],
        'length' => intval($temp[4]),
        'favorite' => intval($temp[5]),
        'view' => intval($temp[6]),
        'author' => $temp[7],
        'rank' => floatval($temp[8]),
    ];
}

switch ($_GET['rank']) {
    case 'title':
        usort($result, function ($a, $b) {
            return strcmp($a['title'], $b['title']);
        });

        break;

    case 'count':
        usort($result, function ($a, $b) {
            return $b['view'] <=> $a['view'];
        });

        break;
}

echo json_encode($result);
