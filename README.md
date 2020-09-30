# Basic Lane Detection


## Process
1. First input the image

```cpp
img = imread(test3.png);
```
![img1](pics/README_pics/img1.jpg)

2\&gt;Perform lane detection on the image and store the final image into dst

\&gt;lane detection already explained in the previous given task:

cvtColor(img, greyscale\_img, COLOR\_BGR2GRAY);

blur(greyscale\_img, greyscale\_\_blur\_img, Size(3, 3));

Canny(greyscale\_\_blur\_img, greyscale\_\_blur\_img, 100, 300, 3);

dst =Scalar::all(0);

img.copyTo(dst, greyscale\_\_blur\_img);

(dst.png) ![](RackMultipart20200930-4-bvionh_html_9c39001d9a1f548.png)

3\&gt;Process for edge detection begins:

First to make sure that the image is greyscale we convert the image into greyscale ,as edge detection is only possible on greyscale image.

Mat inputImage = dst;

if (inputImage.channels() \&gt; 1)

{

cv::cvtColor(inputImage, inputImage, CV\_RGB2GRAY);

}

**4\&gt;** Here we define the Region of an interest in an image .It is basically the region in the whole picture where we have to detect the lanes.I chose a ROI as a triangle in front of the camera of a car to detect the lanes.Hence the images which are taken from the perspective of a car&#39;s front dash cam perform better in the test.

\&gt;For defining ROI.First I declare the co-ordinates;

int **x0 = img.cols - 1;**

int y0 = img.rows - 1;

int x1 = img.cols / 2-1;

int y1 = img.rows / 2-1;

int x2 = 0;

**int**  **y2 = img.rows – 1;**

Then I make these lines on a black background and store in it LineMask image

cv::Mat lineMask = cv::Mat::zeros(inputImage.size(), inputImage.type());

cv::line(lineMask, cv::Point(x0, y0), cv::Point(x1, y1), cv::Scalar(255, 255, 0), 1, 8, 0);

cv::line(lineMask, cv::Point(x0, y0), cv::Point(x2, y2), cv::Scalar(255, 255, 0), 1, 8, 0);

cv::line(lineMask, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(255, 255, 0), 1, 8, 0);

(lineMask.jpg) ![](RackMultipart20200930-4-bvionh_html_4a864e9b7e64b66c.jpg)

5\&gt;Now ,In order to Crop the image in this area,we will have to create a binary mask.

To create a binary mask,we have to perform contour detection on the image. This basically identifies the shape present in the image.It only function precisely in a black and white image ,hence linemask was made by white lines on a black background;

vector\&lt;vector\&lt;Point\&gt;\&gt; contours;

vector\&lt;cv::Vec4i\&gt; hierarchy;

cv::findContours(lineMask, contours, hierarchy, CV\_RETR\_TREE, CV\_CHAIN\_APPROX\_SIMPLE, cv::Point(0, 0));

\&gt;After this test we perform a point polygon test ,which basically tells us which points lie inside the detected object and which points lie outside.We capture all this data in a matrix called raw\_dist;

cv::Mat raw\_dist(lineMask.size(), CV\_32FC1);

for (int i = 0; i \&lt; lineMask.rows; i++)

{

for (int j = 0; j \&lt; lineMask.cols; j++)

{

raw\_dist.at\&lt;float\&gt;(i, j) = cv::pointPolygonTest(contours[0], cv::Point2f(j, i), true);

//cout \&lt;\&lt; raw\_dist.at\&lt;float\&gt;(i, j) \&lt;\&lt; endl;

}

}

\&gt;Using the data stored in the raw\_dist matrix,we create the required binary matrix(mask);

cv::Mat mask = cv::Mat::zeros(inputImage.size(), CV\_8UC1);

for (int i = 0; i \&lt; mask.rows; i++)

{

for (int j = 0; j \&lt; mask.cols; j++)

{

if (raw\_dist.at\&lt;float\&gt;(i, j) \&lt; 0)//point outside the mask

{

mask.at\&lt;uchar\&gt;(i, j) = static\_cast\&lt;uchar\&gt;(0);

continue;

}

mask.at\&lt;uchar\&gt;(i, j) = static\_cast\&lt;uchar\&gt;(255);//point inside the mask

}

}

![](RackMultipart20200930-4-bvionh_html_68fb3f1bc40813fd.png)

(mask.jpg)

6\&gt;We now crop the required region of interest using the above mask from the image and store it into output image;

cv::Mat invInput = inputImage;

cv::Mat outputImage;

invInput.copyTo(outputImage, mask);

![](RackMultipart20200930-4-bvionh_html_889a1c24665c10c8.png)

(outputImage.jpg)

7\&gt;Now we use hough transform to detect any straight lines present in the image and store the end points of the lines in a vector

vector\&lt;Vec4i\&gt; linesP; // will hold the results of the detection

HoughLinesP(outputImage, linesP, 1, CV\_PI / 180, 20, 20, 30);// runs the actual detection

8\&gt;Now I have used the endpoints to calculate slope to avoid a line that is too horizontal to get printed

Now display all the detected line on the initial image

double slope\_thresh = 0.3;

for (size\_t i = 0; i \&lt; linesP.size(); i++)

{

Vec4i l = linesP[i];

double slope = (static\_cast\&lt;double\&gt;(l[3]) - static\_cast\&lt;double\&gt;(l[1])) / (static\_cast\&lt;double\&gt;(l[2]) - static\_cast\&lt;double\&gt;(l[0]) + 0.00001);

if (std::abs(slope) \&gt; slope\_thresh)

line(img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE\_AA);

}

![](RackMultipart20200930-4-bvionh_html_ff00e4df9b40e256.png)

(test3\_output.jpg)
