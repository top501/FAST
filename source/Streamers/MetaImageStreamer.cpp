#include "MetaImageStreamer.hpp"
#include "MetaImageImporter.hpp"
#include "DeviceManager.hpp"
#include "Exception.hpp"
using namespace fast;

/**
 * Dummy function to get into the class again
 */
inline void stubStreamThread(MetaImageStreamer * streamer) {
    streamer->producerStream();
}

DynamicImage::pointer MetaImageStreamer::getOutput() {
    mOutput->setSource(mPtr.lock());
    mOutput->setStreamer(mPtr.lock());
    return mOutput;
}

MetaImageStreamer::MetaImageStreamer() {
    mOutput = DynamicImage::New();
    mStreamIsStarted = false;
    mIsModified = true;
    thread = NULL;
    mFirstFrameIsInserted = false;
    mHasReachedEnd = false;
    mFilenameFormat = "";
    mDevice = DeviceManager::getInstance().getDefaultComputationDevice();
}


void MetaImageStreamer::execute() {
    mOutput->setSource(mPtr.lock());
    mOutput->setStreamer(mPtr.lock());
    if(mFilenameFormat == "")
        throw Exception("No filename format was given to the MetaImageStreamer");
    if(!mStreamIsStarted) {
        mStreamIsStarted = true;
        thread = new boost::thread(&stubStreamThread, this);
    }

    // Wait here for first frame
    // TODO use condition variable instead
    while(!mFirstFrameIsInserted);
}

void MetaImageStreamer::setFilenameFormat(std::string str) {
    if(str.find("#") == std::string::npos)
        throw Exception("Filename format must include a hash tag # which will be replaced by a integer starting from 0.");
    mFilenameFormat = str;
}

void MetaImageStreamer::setDevice(ExecutionDevice::pointer device) {
    mDevice = device;
}

inline std::string intToString(int number) {
    std::stringstream ss;//create a stringstream
    ss << number;//add number to the stream
    return ss.str();//return a string with the contents of the stream
}

void MetaImageStreamer::producerStream() {
    int i = 0;
    while(true) {
        std::string filename = mFilenameFormat;
        filename.replace(
                filename.find("#"),
                1,
                intToString(i));
        try {
            MetaImageImporter::pointer importer = MetaImageImporter::New();
            importer->setFilename(filename);
            importer->setDevice(mDevice);
            Image::pointer image = importer->getOutput();
            image->update();
            DynamicImage::pointer ptr = mOutput;
            if(ptr.isValid()) {
                try {
                    ptr->addFrame(image);
                } catch(Exception &e) {
                    std::cout << "streamer has been deleted, stop" << std::endl;
                    break;
                }
                mFirstFrameIsInserted = true;
            } else {
                std::cout << "DynamicImage object destroyed, stream can stop." << std::endl;
                break;
            }
            i++;
        } catch(FileNotFoundException &e) {
            if(i > 0) {
                std::cout << "Reached end of stream" << std::endl;
                mHasReachedEnd = true;
                // Reached end of stream
                break;
            } else {
                throw e;
            }
        }
    }
}

MetaImageStreamer::~MetaImageStreamer() {
    if(mStreamIsStarted) {
        std::cout << "Joining the thread" << std::endl;
        // TODO stop thread as well
        thread->join();
        delete thread;
    }
}

bool MetaImageStreamer::hasReachedEnd() const {
    return mHasReachedEnd;
}
