# What is Seam Carving?

Seam-carving is a content-aware image resizing technique where the image
is reduced in size by one pixel of height (or width) at a time.

A vertical seam in an image is a path of pixels connected from the top to
the bottom with one pixel in each row.

A horizontal seam is a path of pixels connected from the left to the right
with one pixel in each column.

Steps:
  
  ○ Energy Calculation: Each pixel has some RGB values. Calculate
energy for each pixel. For ex.- You can use a dual-gradient energy
function but you are free to use any energy function of your choice.
Also, you can refer to this link for details.
  
  ○ Seam Identification: Identify the lowest energy seam.
  
  ○ Seam Removal: Remove the lowest energy seam.
