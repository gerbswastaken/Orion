## Orion: A Simple Raycasting Engine
<figure>
  <img src="https://user-images.githubusercontent.com/73233616/176483847-35378b7b-848b-4e03-a888-46c1856a5cb1.png" width="640" height = "480">
  <figcaption> <center> A still from the Orion Engine. </center> </figcaption>
</figure>

# What is Raycasting?
<p>
Simulating 3D graphics without the help of a dedicated graphics card is quite a challenging task, given that vast amounts of calculatioins need to 
be done for each pixel. Performing 3D rendering in realtime was an even more herculean task. ID Software's John Carmack pioneered the process of
raycasting, a method by which somewhat believeable looking 3D graphics could be generated with relative ease.
</p>
<img src="https://user-images.githubusercontent.com/73233616/176487581-a99957a7-5f67-4697-8658-7b4857e78d9b.png" width="320">
A Screencap from ID Software's Wolfenstein, the game that pioneered this method of graphics rendering.
<p>

Raycasting is often thought of as a simplified case of Raytracing; specifically a case where certain assumptions are made about the shapes and 
objects that are going to be rendered. While Raytracing performs calculations for every single pixel, Raycasting performs calculations for only every
column of pixels, and uses the assumptions that: 1.<i> Every wall is at 90° with each other </i> and 2.<i> Walls are simple 3D cubes with a fixed 
height </i> to extrapolate the rest of the pixel data.
</p>

> To learn more about Raytracing, check out my other project, where I implement a Raytracer using C++:
> https://github.com/gerbswastaken/RayTracer-Challenge

# What is the Goal of this Project?
<p> 
The previously mentioned Raytracing project that I made had a lot of issues; chief amongst them was that rendering images took lots of time, and the
objects that could be added to the scene could only have a handful of implemented textures, not a user defined one. I have been interested in 
Game Development for a long time, and have wanted to create a realtime raytracing engine for a long time. This is one step closer towards that goal.

I've learned a lot about programming large projects while writing this Raycasting engine. I made it a point to program <i>every single</i> apsect of
the engine from scratch, from the routines to import <code> .bmp </code> files all the way to the actual routines that determine the intersection 
data to be fed to the rendering function. From doing this, I've learned a lot about planning out projects, and about every single aspect of writing
a realtime rendering engine.

I want to use this Raycasting engine to make and release a game to the public. Though this idea is still in the early stages, I plan to add
a randomly generated map, sound, and overall just optimize the rendering routines to improve framerates.
</p>

# Features 
<ul>
  <li> Textured Map Walls, Floors, and Ceilings. </li>
  <li> Mouse-look along vertical and horizontal axes. </li>
  <li> Routines to convert <code>.bmp</code> texture files into hexadecimal colors. </li>
  <li> Realistic player movement with friction and momentum </li>
  <li> Written in pure C++ (with no libraries outside of the standard C++ libraries) </li>
<ul>
