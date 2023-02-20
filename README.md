# CollisionDetector2D
By Eduardo Machado Martins and Nathan dos Reis Ramos de Mello

## Description
This application is a 2D collision detector that uses OpenGL to create a screen based on a file or on a random generation of points. The field of vision can be manipulated with '←','↑','→' and '↓' keys. As the field of view is moved across the screen, the number of points inside the triangle is calculated. This applications uses three different methodologies to find the answer, the brute force, the envelope and the quadtree algorithms, the algorithm used can be changed using the 'x' key. Use the ' ' key to view implementation details, and 'c' to add colors to the application. Lastly, the triangle size can be incremented or decremented with the '+' and '-' keys respectively.

## Dependencies
This application works on Linux and uses OpenGL libraries, C and C++. When downloaded, the lib.zip and include.zip files must be unzipped in the root directory.

## Execution
The following command compiles the application.
```
make
```
The following command execute the application.
```
./app
```

##
<div align="center">  
  <img src="https://cdn.discordapp.com/attachments/1076157666986049598/1076849328829771826/colider2d.png" alt="Image preview" /> 
</div>
