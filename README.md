# 1BRC: One Billion Row Challenge in PHP

Your mission, should you decide to accept it, is deceptively simple: write a Java program for retrieving temperature measurement values from a text file and calculating the min, mean, and max temperature per weather station. There’s just one caveat: the file has 1,000,000,000 rows!

The text file has a simple structure with one measurement value per row:

```sh
Hamburg;12.0
Bulawayo;8.9
Palembang;38.8
St. John's;15.2
Cracow;12.6
```

The program should print out the min, mean, and max values per station, alphabetically ordered like so:

```sh
{Abha=5.0/18.0/27.4, Abidjan=15.7/26.0/34.1, Abéché=12.1/29.4/35.6, Accra=14.7/26.4/33.1, Addis Ababa=2.1/16.0/24.3, Adelaide=4.1/17.3/29.7, ...}
```

This is almost exclusively taken from/based on work by Florian Engelhardt:

[https://github.com/realFlowControl/1brc] (https://github.com/realFlowControl/1brc)


He also wrote a blog about this: [Processing One Billion Rows in PHP!](https://dev.to/realflowcontrol/processing-one-billion-rows-in-php-3eg0)

# Usage

```sh
make average
# or
php createMeasurements.php 1000000000
php -d extension=parallel calculateAverage.php > average.txt
```

# Requirements

This solutions requires a ZTS build of PHP and
[`ext-parallel`](https://github.com/krakjoe/parallel) to be installed for that
PHP version.
