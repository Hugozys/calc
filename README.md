![workflow](https://github.com/Hugozys/calc/workflows/calc-ci/badge.svg)

# Simple Calculator

## Build From Source

You need to have cmake installed on your localhost to build this from source
Under the project root directory, run:

```bash
mkdir build && cd build && cmake .. -DFRUIT_USES_BOOST=False && make
```

These commands will use cmake to create Makefile which will first build the fruit as a shared object and then link it to the calculator app.
You should be able to find an executable called `calc` under `calc/bin` directory.

See [notes](./notes.md) for more information.
