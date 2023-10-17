# keycounts

Record keyboard keypress counts over time.

This utility monitors all keyboard inputs attached to a Linux computer and
records the count of each time any keys are pressed. No timing information is
stored, nor order of key pressed, so it should not be possible to use to
recover any specific input from the stored data once it has been in use for a
while. Nevertheless, please consider carefully whether this is something you
feel comfortable using as it may be possible to use it as a general key logger
in a limited fashion.

The aim of this utility is to figure out which keys are used most often. The
impetus for writing it was to help configure the keymap of a Reviung 34 key
keyboard, knowing that the author was a programmer and heavy vim user, so would
make liberal use of symbols and the Esc key, and so the default keymap may not
be suitable. This experiment is still ongoing.

Only the "key down" event is considered, meaning that individual key presses
are detected but if you hold down a key to produce a repeating series of
letters that is not detected. The aim is, in effect, to detect what the fingers
are moving to only.

The data is stored by default at `/var/lib/keycounts`

## Compilation and installation

Run:

```
make
sudo make install
```

You will need make, gcc, and the Linux kernel headers at a minimum.

## Recording counts

Running manually:

```
sudo keycounts record
```

Running via systemd:

```
sudo cp keycounts.service /etc/systemd/system/
systemctl enable keycounts
systemctl start keycounts
```

Record to a specific file:

```
sudo keycounts record -f /my/data/file
```

Record key presses and print the key labels to stdout, to help with figuring
out which key is which:

```
sudo keycounts record -v
```

## Reporting counts

```
keycounts report
```

Sorting is nice:

```
keycounts report | sort -h
```

## Generating results images

keycounts can generate svg files showing a heat map of key use.

```
keycounts image > file.svg
```

The default (and only) layout is a standard UK desktop keyboard. It is possible
to use different layouts by creating new SVG header/footer files as in the
`keyboards` directory.

## Snap package

keycounts is available as a snap package. To use it, you must connect the `raw-input` plug:

```
snap connect keycounts:raw-input :raw-input
sudo snap restart keycounts
```

The report and image commands become `keycounts.report` and `keycounts.image`
respectively.


## Similar projects

* https://github.com/illdefined/keystat
  Focused on timing statistics

* https://github.com/ne0phyte/keystats
  X-Windows applications only.
