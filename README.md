# KMP Empirical Study

Programming Assignment 1 - Track A: Implementation and Empirical Study

## Overview

This project implements the Knuth-Morris-Pratt (KMP) string matching algorithm and compares its practical performance against a naive substring search baseline.

KMP improves worst-case string searching by using information about previously matched characters. It preprocesses the pattern into an LPS (Longest Proper Prefix which is also a Suffix) table, allowing the search to avoid restarting from the beginning of the pattern after every mismatch.

The empirical study investigates how KMP and naive search behave under different types of input, including random, repetitive, and adversarial strings.

## Research Question

How does KMP compare with naive substring search across different input sizes and string structures, and when does KMP's theoretical advantage become visible in practice?

## Algorithms

### Naive String Search

The naive algorithm tries the pattern at every possible starting position in the text.

If the text has length `n` and the pattern has length `m`, its worst-case running time is:

O(nm)

### Knuth-Morris-Pratt

KMP first builds an LPS table for the pattern.

The LPS table records the length of the longest proper prefix of each pattern prefix that is also a suffix.

When a mismatch occurs, KMP uses this table to determine how much of the previous match can still be reused instead of restarting from the beginning.

Its worst-case running time is:

O(n + m)

## Video Walkthrough

The video walkthrough is included in as:

`KMP_video.mp4`

Note: There wasn't audio when viewing the video inside of Visual Studio Code, please download for the video. Otherwise happy to make an unlisted video upon contact.