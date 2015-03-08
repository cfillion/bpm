import qbs 1.0

CppApplication {
  name: "Blue Pink Metronome"

  cpp.cxxFlags: [
    "-fcolor-diagnostics",
    "-std=c++11",
  ]

  files: [
    "resources/main.qrc",
    "src/gui.*",
    "src/main.cpp",
    "src/metronome.*",
  ]

  Depends { name: "Qt"; submodules: [
    "core",
    "widgets",
    "multimedia",
  ]}

  Group {
    fileTagsFilter: "application"
    qbs.install: true
    qbs.installDir: "bin"
  }
}
