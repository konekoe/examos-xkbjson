# xkbjson - JSON-friendly xkb utility

Part of Konekoe's ExamOS application bundle



### Usage

To list (as JSON) all possible layouts and variants:

```
$ xkbjson -l 
```

To get the current layout (as JSON):

```
$ xkbjson -c
```


To set new xkb layout and variant (with no variant given, default is used):

```
$ xkbjson -s layout <layout> [variant <variant>]
```

or in GNU-style

```
$ xkbjson --set layout <layout> [variant <variant>]
```