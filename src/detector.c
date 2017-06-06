#include "network.h"
#include "region_layer.h"
#include "utils.h"
#include "parser.h"
#include "option_list.h"
#include "socket_work.h"

/**
 * Detection of objects in the image
 * @param datacfg - label file
 * @param cfgfile - neural network configuration file
 * @param weightfile - file with weights
 * @param filename - image file
 * @param thresh - detection threshold
 * @param hier_thresh - localization threshold
 */
void run_detector(char *datacfg, char *cfgfile, char *weightfile, char *filename, float thresh, float hier_thresh)
{
    // parse parameters of a neural network
    list *options = read_data_cfg(datacfg);
    char *name_list = option_find_str(options, "names", "data/names.list");
    char **names = get_labels(name_list);
    image **alphabet = load_alphabet();
    network net = parse_network_cfg(cfgfile);
    char submit[256] = "Parsing configuration is complete";
    send_message(submit);
    // load of weights
    if(weightfile){
        load_weights(&net, weightfile);
        strcpy(submit, "Loading of weights finished");
        send_message(submit);
    }
    set_batch_network(&net, 1);
    srand(2222222);
    clock_t time;
    char input[256];
    int j;
    float nms=.4;
    while(1){
        // image upload
        strncpy(input, filename, 256);
        image im = load_image_color(input,0,0);
        image sized = letterbox_image(im, net.w, net.h);
        strcpy(submit, "Loading of image finish");
        send_message(submit);
        // initializing boxes and probabilities
        layer l = net.layers[net.n-1];
        box *boxes = calloc(l.w*l.h*l.n, sizeof(box));
        float **probs = calloc(l.w*l.h*l.n, sizeof(float *));
        for(j = 0; j < l.w*l.h*l.n; ++j) probs[j] = calloc(l.classes + 1, sizeof(float *));
        // initialization of input data
        float *X = sized.data;
        char time_info[256];
        bzero(time_info, 256);
        strcpy(submit, "Start predict");
        send_message(submit);
        time=clock();
        // predict
        network_predict(net, X);
        sprintf(time_info, "Predicted in %f seconds.\n", sec(clock()-time));
        send_message(time_info);
        printf(time_info);
        remove(filename);
        // оbject rendering
        get_region_boxes(l, im.w, im.h, net.w, net.h, thresh, probs, boxes, 0, 0, hier_thresh, 1);
        if (do_nms_obj) do_nms_obj(boxes, probs, l.w*l.h*l.n, l.classes, nms);
        draw_detections(im, l.w*l.h*l.n, thresh, boxes, probs, names, alphabet, l.classes);
        save_image(im, "predictions");
        strcpy(submit, "predictions.png");
        send_image(submit);
        // release of resources
        free_network(net);
        free_image(im);
        free_image(sized);
        free(boxes);
        free_ptrs((void **)probs, l.w*l.h*l.n);

        if (filename) break;
    }
}
