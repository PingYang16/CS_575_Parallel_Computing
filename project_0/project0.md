---
title: "Project #0 Simple OpenMP Experiment"
author: Ping Yang
date: Apr 4, 2022
output: pdf_document
---

# 1. Tell what machine you ran this on.

I ran this on the engr sever flip with a ArraySize = 16328, and NUMTRIES = 1000.

# 2. What performance results did you get?

- With use of 1 thread, the performance result is 311.18 MegaMults/Sec.
- With use of 4 threads, the performance result is 1123.20 MegaMults/Sec.

# 3. What was your 4-thread-to-one-thread speedup?

$$S = \frac{\text{Performance with four threads}}{\text{Performance with one thread}} = \frac{1123.20}{311.18} = 3.609$$

# 4. If the 4-thread-to-one-thread speedup is less than 4.0, why do you think it is in this way?

The 4-thread-to-one-thread speedup is less than 4.0 is because the algorithm is not been optimized then it will spend some time to switch between different threads. Also, some threads may occupy the memory and other threads need to wait for the former. So, the 4-thread-to-one-thread speedup cannot reach 4.0.

# 5. What was your Parallel Fraction, Fp?

The Parallel Fraction of mine is:
$$Fp = \frac{4.0}{3.0}\cdot(1.0-\frac{1.0}{S}) = \frac{4.0}{3.0}\cdot(1.0-\frac{1.0}{3.609}) = 0.9639$$