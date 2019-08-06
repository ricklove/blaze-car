# Blaze Car

Blazing Fast Self Driving RC Car 

200FPS Image Analysis on Raspberry Pi


## Summary

Full Convolutional Neural Networks are simply too much processing for a Raspberry Pi 3B - especially at 200FPS. 

A more custom approach is needed running on a constrained environment.


## Goals

- Support speeds of at least 30mph
- Adapt to any visual conditions:
  - Indoor support with glare from indoor lighting
  - Outdoor support with glare from sun and shade over the course
- Learn extreme driving modes like sliding turns and drifting 
- Map course solely from visual input



## Modules

- Pixel Online Learning Module
  - Learn Pixel Colors Online (during runtime) to adjust to lighting conditions
  - To ensure stability, use only verified results - strategically using a more expensive algorithm like edge detection to confirm learning results




# To Build and Run

  - ./buildme
  - ./run
