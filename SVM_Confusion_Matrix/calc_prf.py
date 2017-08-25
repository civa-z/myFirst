#coding=utf-8

### calculate Precision, Recall, and F-Measure
'''
how to use:

cat test_epoch50.log | python2 ./calc_prf1.py 17

----------------
17: classes


.log file:
    left: predicted class

    right: ground truth


note:
classes label should start from 0.
'''


import numpy as np
import sys
reload(sys)
sys.setdefaultencoding("utf-8")

# explanation of sys.argv:
### e.g.: cat test_epoch50.log | python2 ./calc_prf_hu.py 17
### sys.argv[1]: get the parameter 17
nlbls = int(sys.argv[1])  # label ������

true_ls = []
pred_ls = []

while True:
    line = sys.stdin.readline()
    if not line:
        break
    line = line.strip()
    if not line:
        continue
    ls = line.split()
    true_ls.append(int(float(ls[1])))
    pred_ls.append(int(float(ls[0])))

assert len(true_ls) == len(pred_ls)
cnt = len(true_ls)
print "true_len: ", cnt  # the length of true label
print "nlbls: ", nlbls   # the number of label class

# confusion matrix
cm = np.zeros(shape=(nlbls, nlbls), dtype=np.int32)
for i in range(cnt):
    cm[true_ls[i], pred_ls[i]] += 1
print cm

# calculate the accuracy
acc_abs = 0
for i in range(cnt):
    if true_ls[i] == pred_ls[i]:
        acc_abs += 1
print "Acc.", acc_abs*1.0/cnt


marginal_pred = cm.sum(axis=0)
marginal_true = cm.sum(axis=1)
for i in range(nlbls):
    if 0 == marginal_pred[i]:
        continue
    p = cm[i, i]*1.0 / marginal_pred[i]  # Precision
    r = cm[i, i]*1.0 / marginal_true[i]  # Recall
    f = 2*p*r / (p+r) # F-Measure

    # print i, p*100, r*100, 200*p*r/(p+r)
    print i, p*100, r*100, 100*f
