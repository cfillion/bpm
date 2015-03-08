# Blue Pink Metronome

![Screenshot](http://i.imgur.com/tSkQreZ.png)

## Installation

Dependencies:

- Qt 5 (core, widgets and multimedia)
- qbs (tested with clang)

```sh
$ qbs release
```

If you haven't configured qbs to build Qt projects, check these [instructions](http://doc.qt.io/qbs/configuring.html).

## Usage

BPM is controlled by the keyboard. Here is the list of available actions:

- **&lt;Space&gt;** &mdash; Start or stop the metronome
- **&lt;Up Arrow&gt;** &mdash; Increase the speed by 10 bpm
- **&lt;Right Arrow&gt;** &mdash; Increase the speed by 1 bpm
- **&lt;Left Arrow&gt;** &mdash; Decrease the speed by 1 bpm
- **&lt;Down Arrow&gt;** &mdash; Decrease the speed by 10 bpm
- **1**&ndash;**9** &mdash; Set how many beats each measure has
- **t** &mdash; Tap Tempo
- **q**, **&lt;Return&gt;**, **&lt;Escape&gt;** &mdash; Exit
