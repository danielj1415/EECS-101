# EECS-101
Folder containing all project assignments from the EECS 101: Computer Vision course at the University of California, Irvine.


# EECS101: Homework #1
This assignment focuses on generating images that demonstrate the differences between perspective and orthographic projections of 3D lines. You will work with a provided C program to modify image coordinate calculations and explore how these projections transform lines in three-dimensional space onto a two-dimensional image plane.

In this project, you will write expressions for the image coordinates 
𝑥′x' and 𝑦′y′
based on given parameters, and implement these in the C program. By running the program with various settings, you'll generate images for both types of projections and analyze the resulting patterns, such as whether lines remain straight or parallel under different projections.

Additionally, you will investigate the behavior of the perspective projection as certain parameters approach infinity and compare it to the orthographic projection, considering whether one can approximate the other under specific conditions. The final submission will include the modified code, generated images, and a demonstration of your work during the lab session.


# EECS 101: Homework #2
In this assignment, you will explore the interplay between signal and noise in a cooled CCD camera system and analyze the effects of imaging system configurations on image quality. The first problem focuses on deriving the variance of an amplified measurement at a pixel site, where the measurement is influenced by signal and independent noise sources. You will calculate the signal-to-noise ratio and determine the minimum signal level required to achieve a specified ratio, providing insight into the behavior of CCD cameras under different conditions.

The second problem involves understanding how lens characteristics and image plane configurations affect the focus and resolution of images. You will calculate the precise position of a point source in front of a lens to achieve a clear, unblurred image on the image plane. Additionally, you'll determine the extent to which the point can be moved before the image blurs across multiple pixels, considering the relationship between focal length, lens diameter, and pixel size.

Finally, in the third problem, you will extend your analysis to a CCD camera system where the noise due to dark current is negligible. You will derive expressions for the expected value and variance of digitized pixel values, and implement a program to read and analyze digital images. By plotting variance against mean values and performing a least squares fit, you will estimate key parameters of the imaging system, deepening your understanding of how noise and amplification influence image data.


# EECS 101: Homework #3
This assignment is divided into two sections: a written problem and a computer problem, both centered around image processing techniques. The written problem focuses on the region growing method, a fundamental technique in image segmentation. You will apply a homogeneity measure to identify and grow regions in a grayscale image, starting from the upper left pixel. The task involves calculating the mean and sample variance of the growing region and determining the point at which the variance exceeds a predefined threshold. This exercise provides hands-on experience in understanding how region-based segmentation works and its application to simple grayscale images.

In the computer problem, you will work with three 512 × 512 grayscale images to explore thresholding techniques, a basic but powerful method for image segmentation. Your tasks include displaying the grayscale images, generating and analyzing their histograms, and selecting appropriate thresholds to separate objects from the background. You will then implement a program that converts the grayscale images into binary images, where the object is highlighted against the background. The program will also calculate and display the object's area and center of area, allowing you to visualize and verify the accuracy of your segmentation.

Upon completion of this assignment, you will submit the binary images generated from your program, along with the selected thresholds, object areas, and object centers of area. This assignment aims to strengthen your understanding of image processing, particularly in segmentation techniques, by providing practical experience in both manual calculations and programming tasks.


# EECS 101: Homework 4
In this assignment, you will explore edge detection techniques using the Sobel operator, a widely-used method in image processing for identifying edges in digital images. The assignment requires you to process three provided grayscale images, applying the Sobel operator to detect edges along the x and y directions. The first step involves displaying the original grayscale images to establish a baseline for comparison. Following this, you will generate two separate images that highlight the edges detected along the x-axis and y-axis, respectively. These edge-detected images are normalized so that their maximum pixel value is 255, ensuring that the edges are clearly visible.

The next step involves combining the x and y directional gradients to create an image representing the squared gradient magnitude. This image highlights the overall strength of edges in the original images by capturing the combined effect of changes in both directions. By normalizing this image in a similar fashion, you make the edges prominent for further analysis. Finally, you will apply a threshold to this squared gradient magnitude image to produce a binary image that isolates the edges. This thresholding step is crucial for simplifying the image, reducing it to a clear, binary form where edges are distinctly marked.

For each of the three images, you will generate and display four processed images: the edge-detected images along the x and y directions, the squared gradient magnitude image, and the binary edge image. The assignment reinforces your understanding of edge detection, gradient operations, and the application of the Sobel operator. You will submit your code along with the twelve images generated through these tasks, demonstrating your ability to implement and apply edge detection algorithms in image processing.


# EECS 101: Homework 5
In this assignment, you will implement the Hough transform to detect straight-line edges in a given image, building on the edge detection techniques from Homework #4. The process begins with computing the gradients of the image using the Sobel operator to obtain the partial derivatives ∂E/∂x and ∂E/∂y at each pixel. From these gradients, you will calculate the squared gradient magnitude (SGM) for each pixel to identify potential edges. A threshold will be applied to the SGM to isolate significant edges from the background noise.

Once the edges are isolated, the next step involves using the gradient vector and the edge position to determine the parameters (ρ, θ) for the lines that these edges may represent. The edge direction is perpendicular to the gradient vector, and these parameters are essential for the Hough transform. The Hough transform will then be used to find and highlight linear edges in the image by applying a threshold to the (ρ, θ) voting array. This technique transforms the edge detection problem into a voting problem in the parameter space, allowing for the identification of lines.

You will submit your program along with the results, which include a binary image showing the thresholded SGM, and a binary image that highlights the detected lines from the Hough transform. Additionally, provide the computed values for ρ and θ for each detected line, including the number of edge pixels (votes) that contributed to each line. Your submission should demonstrate the effective application of the Hough transform for line detection and highlight your ability to integrate gradient-based edge detection with parameter space analysis.

# EECS 101: Homework 6
In this homework assignment, you will calculate various geometric properties related to solid angles and light reflection in different scenarios. The first task involves determining the solid angle subtended by two different objects as viewed from a given distance. For the moon, which is modeled as a sphere of radius 
R at a distance 𝑑 from Earth, you will compute the solid angle it subtends. Similarly, you will find the range of possible solid angles for a flat circular plate of the same radius 
𝑅 at the same distance 𝑑d.

The second task examines a cubic room with dimensions of 100 feet on each side, focusing on a 1 foot by 1 foot square patch on the ceiling. You are required to calculate the foreshortened area of this patch when viewed from a corner of the room on the floor and determine the solid angles subtended by the patch from both the floor corner and a corner on the ceiling. These calculations will involve understanding how perspective affects the apparent size and angular measurement of the patch from different viewpoints.

Finally, the third task involves analyzing a Lambertian plane defined by the equation 
7x+50 y+z+2=0. You will find the surface gradient of the plane and determine the optimal location for a point light source such that the light source is 20 units away from a specific point 
𝑃=(0,0,−2) on the plane. The goal is to maximize the reflected radiance from 
𝑃P in the direction of the origin (0,0,0). This task combines geometric analysis with principles of radiometry and light reflection.


# EECS 101: Homework 7
In this assignment, you will develop a program to generate images of a sphere under orthographic projection, utilizing a reflectance model. The sphere is represented with its center aligned on the optical axis, and you will calculate the surface normal vector for each point on the sphere's surface. This normal vector is essential for determining how light interacts with the sphere, affecting its appearance in the image.

The sphere will be illuminated by point sources, and the program will simulate how the sphere’s surface reflects light. The camera direction is fixed, simplifying the calculations. The radiance of the sphere is a combination of Lambertian reflection, which depends on the angle between the light source and the surface normal, and specular reflection, which is influenced by the smoothness of the surface. The reflectance model you will use blends these two components to simulate realistic lighting effects.

Your task is to generate images of the sphere by applying the reflectance model across its surface and adjusting the brightness values to fit the 8-bit pixel range. You will work with different light source positions while keeping other parameters constant. Be sure to submit the generated images along with your code and any observations on how the parameters affect the final images.



