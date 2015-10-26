#Detection
##Import Image
```python
```

##Edge Filtering
```python
```

##Find Contours
```python
```

##Find Focus Points
```python
```

##Classify Focus Points
```python
```

##Sort Corners
The idea behind this method is to sort 4 corners radially by their angle from their mutual centroid.
```python
```

##Get Centriod
Returns the center point within a focus point.
```python
```

##Get Filled
Returns whether or not the center area of a focus point is "filled."
```python
```

##Detection
Our final detection method, returning four sorted corners within an image, then becomes:
```python
```
Also, returning which focus points are filled:
```python
```

##(Optional) Webcam Test
```python
```

##(Optional) 
##(Optional) Draw Contours
```python
```

#Capturing
##Warp Perspective
```python
```

##Sharpness Detection
```python
```

##Skew Detection
```python
```

##Text Notes Filter
```python
```

##Save Image
```python
```

##Capture
Finally, our main capture method becomes:
```python
```