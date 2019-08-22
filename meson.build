qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: ['Core', 'Gui'])

inc = include_directories('src/helpers')

source_files = [
  'src/main.cpp',
  'src/mainwindow.cpp'
]

header_files = [
  'src/mainwindow.h',
  'src/meta/note.h'
]

compiled_moc = qt5.preprocess(moc_headers : header_files,
                              moc_extra_arguments: ['-DMAKES_MY_MOC_HEADER_COMPILE'],
                              include_directories: inc,
                              dependencies: qt5_dep)

executable('vibrato', cpp_files, compiled_moc,
           include_directories: inc,
           dependencies : qt5_dep)
