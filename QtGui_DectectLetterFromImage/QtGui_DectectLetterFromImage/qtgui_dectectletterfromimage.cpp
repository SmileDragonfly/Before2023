#include "stdafx.h"
#include "qtgui_dectectletterfromimage.h"

QtGui_DectectLetterFromImage::QtGui_DectectLetterFromImage(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    ui.setupUi(this);
}
void QtGui_DectectLetterFromImage::scaleAndDisplayToGraphicsView(QImage image, GraphicViewsNum graphicsViewNum)
{
    QSize qNewImgSize;
    QGraphicsView* graphicsControl;
    if (graphicsViewNum == GRAPHICSVIEW_INPUT)
    {
        graphicsControl = ui.graphicsView;
    }
    else if(graphicsViewNum == GRAPHICSVIEW_OUTPUT)
    {
        graphicsControl = ui.graphicsViewOut;
    }
    // Calculate scale to fit with graphics view width or height
    float fScale, fWidthScale, fHeightScale;
    fWidthScale = (float)graphicsControl->size().width() / image.size().width();
    fHeightScale = (float)graphicsControl->size().height() / image.size().height();
    fScale = (fWidthScale < fHeightScale) ? fWidthScale : fHeightScale;

    // Calculate new image size
    qNewImgSize.setWidth(fScale * image.size().width() * 0.995);
    qNewImgSize.setHeight(fScale * image.size().height() * 0.995);

    // Scale image to new size
    image = image.scaled(qNewImgSize);

    // Change graphics view size to fit with image
    // ui.graphicsView->resize(newImgSize / 0.995);

    QGraphicsScene* qScene = new QGraphicsScene();
    // Set scene
    graphicsControl->setScene(qScene);
    QGraphicsPixmapItem* qItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    qScene->addItem(qItem);
    graphicsControl->show();
}

void QtGui_DectectLetterFromImage::on_inputImageButton_clicked()
{
    QFileDialog qInputDlg(NULL, "Open Image", "E:/MyProject/OpenCV_ConsoleApp/OpenCV_ConsoleApp", "Image Files (*.jpg *.png)");
    if (qInputDlg.exec())
    {
        // Load image
        m_qImage.load(qInputDlg.selectedFiles()[0]);
 
        // Display image
        scaleAndDisplayToGraphicsView(m_qImage, GRAPHICSVIEW_INPUT);
        ui.inputImageEdit->setText(qInputDlg.selectedFiles()[0]);

        // __[BEGIN PROCESSING]
        // [Process: Detect text here]
        // 1st : Blob detection
        Mat image = imread(qInputDlg.selectedFiles()[0].toStdString(), IMREAD_GRAYSCALE);
        // Setup SimpleBlobDetector parameters.
        SimpleBlobDetector::Params params;

        // Change thresholds
        params.minThreshold = 10;
        params.maxThreshold = 200;

        // Filter by Area.
        params.filterByArea = true;
        params.minArea = 1500;

        // Filter by Circularity
        params.filterByCircularity = true;
        params.minCircularity = 0.1;

        // Filter by Convexity
        params.filterByConvexity = true;
        params.minConvexity = 0.87;

        // Filter by Inertia
        params.filterByInertia = true;
        params.minInertiaRatio = 0.01;

        std::vector<KeyPoint> keypoints;
        // Set up detector with params
        Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

        // Detect blobs
        detector->detect(image, keypoints);
        Mat imageWithKeyPoints;
        drawKeypoints(image, keypoints, imageWithKeyPoints);
        image.release();
        // __[END PROCESSING]

        // Write keypoints image to file, file is saved in input file folder
        QFileInfo qInputFile(qInputDlg.selectedFiles()[0]);
        QString qOutputFile(qInputFile.path() + "/Output.jpg");
        imwrite(qOutputFile.toStdString(), imageWithKeyPoints);
        imageWithKeyPoints.release();
        
        // Display temp file to grapghics output
        m_qImageOutput.load(qOutputFile);
        scaleAndDisplayToGraphicsView(m_qImageOutput, GRAPHICSVIEW_OUTPUT);
    }
}

void QtGui_DectectLetterFromImage::resizeEvent(QResizeEvent *event)
{
    // If this is not initiation
    if (event->oldSize().width() != 0xffffffff)
    {
        // [Resize graphics view]
        QSize oldDlgSize, newDlgSize;
        // Dialog old size and new size
        oldDlgSize = event->oldSize();
        newDlgSize = event->size();

        QSize graphicsViewNewSize;
        // Calculate new size for graphics view
        int newWidth = ui.graphicsView->size().width() + newDlgSize.width() - oldDlgSize.width();
        int newHeight = ui.graphicsView->size().height() + newDlgSize.height() - oldDlgSize.height();

        graphicsViewNewSize.setWidth(newWidth);
        graphicsViewNewSize.setHeight(newHeight);

        // Resize graphics view
        ui.graphicsView->resize(graphicsViewNewSize);
        __trace("newsize = %dx%d\n\n", ui.graphicsView->size().width(), ui.graphicsView->size().height());

        // [Resize image]
        scaleAndDisplayToGraphicsView(m_qImage, GRAPHICSVIEW_INPUT);
    }
}
