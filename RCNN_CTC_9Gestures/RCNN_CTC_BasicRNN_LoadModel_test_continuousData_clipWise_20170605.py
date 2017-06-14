# -*- coding: utf-8 -*-

"""
Load the RCNN model and use it to predict the practice data
"""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import os
os.environ['CUDA_VISIBLE_DEVICES'] = '0'

import codecs
import rcnn_input_data
import tensorflow as tf
import numpy as np

########################
#define the parameter
########################
frame_num = 8
joint_num = 14
joint_dim = 3
n_hidden = 128
n_classes = 9


########################
#define the graph
########################

print ('Defining the graph')
graph = tf.Graph()
with graph.as_default():
    # input
    x = tf.placeholder(tf.float32,shape=[1,frame_num,joint_num,joint_dim])
    state_input = tf.placeholder(tf.float32,shape=[1,n_hidden])
    rnn_out_in_put = tf.placeholder(tf.float32,shape=[1,n_hidden])
    # RCNN variables
    weights = {
        'cnn_c1': tf.Variable(tf.random_normal([3, 3, 3, 32])),
        'cnn_c2': tf.Variable(tf.random_normal([3, 3, 32, 64])),
        'cnn_fc': tf.Variable(tf.random_normal([8 * 14 * 64, n_hidden])),
        'out': tf.Variable(tf.truncated_normal([n_hidden, n_classes+1], stddev=0.1))
    }

    biases = {
        'cnn_c1': tf.Variable(tf.random_normal([32])),
        'cnn_c2': tf.Variable(tf.random_normal([64])),
        'cnn_fc': tf.Variable(tf.random_normal([n_hidden])),
        'out': tf.Variable(tf.constant(0., shape=[n_classes+1]))
    }


    # Define RNN cell
    rnn_cell = tf.nn.rnn_cell.BasicRNNCell(n_hidden, activation=tf.nn.relu6)
    # Stacking rnn cells
    stack = tf.nn.rnn_cell.MultiRNNCell([rnn_cell], state_is_tuple=True)
    init_state = stack.zero_state(1, tf.float32)
    
    #### convolution layer 1
    conv1 = tf.nn.conv2d(x, weights['cnn_c1'], strides=[1, 1, 1, 1], padding='SAME')
    relu1 = tf.nn.relu(tf.nn.bias_add(conv1, biases['cnn_c1']))
    pool1 = tf.nn.max_pool(relu1, ksize=[1, 1, 1, 1], strides=[1, 1, 1, 1], padding='SAME')
    norm1 = tf.nn.local_response_normalization(pool1)

    #### convolution layer 2
    conv2 = tf.nn.conv2d(norm1, weights['cnn_c2'], strides=[1, 1, 1, 1], padding='SAME')
    relu2 = tf.nn.relu(tf.nn.bias_add(conv2, biases['cnn_c2']))
    pool2 = tf.nn.max_pool(relu2, ksize=[1, 1, 1, 1], strides=[1, 1, 1, 1], padding='SAME')
    norm2 = tf.nn.local_response_normalization(pool2)
    
    #### Full connected layer
    pool_shape = pool2.get_shape().as_list()
    reshape = tf.reshape(norm2, [pool_shape[0], pool_shape[1] * pool_shape[2] * pool_shape[3]])
    fc_rnn = tf.matmul(reshape, weights['cnn_fc']) + biases['cnn_fc']
    
    a = tuple([state_input])
    
    with tf.variable_scope('RNN'):
        rnn_out = stack(fc_rnn,a)
    pred = tf.matmul(rnn_out_in_put, weights['out']) + biases['out']
    pred_softmax = tf.nn.softmax(pred)
    pred_topk = tf.nn.top_k(pred_softmax, 3)
    label = tf.arg_max(pred_softmax,1)


#######################################
# Load Data
#######################################

# Load data sets for RCNN
#print('Loading data')
#data, seq_labels, clip_labels, seq_num_raw, flag = rcnn_input_data.load_data('data_9G_continuous/20170526001.data')
# data = rcnn_input_data.data_normalized(data, -1, 1)


# normalization function
def normalize_func(data, lowerBound, upperBound):

    minVal = np.amin(data)
    maxVal = np.amax(data)
    # minVal = np.amin(tr_data)
    # maxVal = np.amax(tr_data)
    # maxVal = 500
    # minVal = -300
    # print('minVal: ', minVal)
    # print('maxVal: ', maxVal)

    def normalizeFunc(x):
        if x>=minVal and x<maxVal:
            r = (x - minVal) * (upperBound - lowerBound) / (maxVal - minVal) + lowerBound
        elif x<0:
            r = -1
        else:
            r = 1 
        return r

    return np.frompyfunc(normalizeFunc, 1, 1)


# generate normalized data
def data_normalized(data, lowerBound, upperBound):

    outufuncXArray = normalize_func(data, lowerBound, upperBound)(data)  # the result is a ufunc object
    dataXArray = outufuncXArray.astype(float)  # cast ufunc object ndarray to float ndarray

    return dataXArray


########################################
# Run Session
#######################################

saver = None
with graph.as_default():
    # Saver for trained model
    saver = tf.train.Saver()

session = tf.Session(graph=graph)
saver.restore(session,  'RCNN_CTC_9Gestures/model/20170526_t1_train_9G_normal_shuffled_epoch200_lr0003_bs125_Basic_RCNN_model.data')
state_feed = session.run(init_state)
def gesture_predictor(feature):
    global state_feed
    if session:
        feature = data_normalized(feature, -1, 1)
        rnn_out_in_put_feed,state_feed = session.run(rnn_out,{x:[feature[3]],state_input:state_feed[0]})
        pred_topk_probabilities, pred_topk_labels = session.run(pred_topk,{rnn_out_in_put:rnn_out_in_put_feed,state_input:state_feed[0]})
        result = []
        for i in range(pred_topk_probabilities.shape[1]):
            result.append([pred_topk_labels[0][i], pred_topk_probabilities[0][i]])
        return result
    return None
