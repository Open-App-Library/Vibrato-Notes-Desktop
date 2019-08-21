src/
---

Welcome to the src directory of Vibrato Notes Desktop Edition!

## Files

There are three main code files in the root of this directory:

- main.cpp
- mainwindow.h
- mainwindow.cpp

main.cpp is the entry point of Vibrato Notes. This main file creates a QApplication and loads the MainWindow.

The two mainwindow files create the main application window - MainWindow.

## Directories

- components - Various components and widgets that make up Vibrato Notes
- helpers - Tiny little files that provide helper functions that make our life easier
- crypto - All the encryption stuffs!
- meta - Definitions of notes, notebooks, and tags. As well as
- sql - Local database storage
- plugin-engine - The scripting/plugin engine!
- cloud-api - Connecting to the Vibrato sync server

## Dictionary

## "Manager"

Examples: SQLManager, TreeManager, EscribaManager

A manager is a class that is able to manipulate a UI component. This manager class provides a high-level API that makes it easy for developers to do cool ass things with the components.
