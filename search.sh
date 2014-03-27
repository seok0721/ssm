#!/bin/bash
find . -name "*" -exec grep -ln "$1" {} \;
