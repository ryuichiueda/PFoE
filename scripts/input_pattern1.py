#!/usr/bin/env python
import sys
import rospy
import time
import random
from t_maze_return_path.srv import ExecAction,ExecActionResponse
from std_msgs.msg import String

from pfoe.srv import EventRegist, FlushData

def flush_data(datatype,filename):
    rospy.wait_for_service('/pfoe/flush_data')
    try:
        p = rospy.ServiceProxy('/pfoe/flush_data', FlushData)
        res = p(filename,datatype)
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e
    else:
        return False

    return True

def act(action):
    rospy.wait_for_service('/t_maze_return_path/action')
    rospy.wait_for_service('/pfoe/event_regist')

    action = "fw"
    r = random.randint(0,2)
    if r == 1:
        action = "cw"
    elif r == 2:
        action = "ccw"

    try:
        exec_action = rospy.ServiceProxy('/t_maze_return_path/action',ExecAction)


        res = exec_action(action)

    except rospy.ServiceException, e:
        print "EXECPTION"


    try:
        sp = rospy.ServiceProxy('/pfoe/event_regist', EventRegist)
	print action, res.sensor, res.reward
        res = sp(action,res.sensor,res.reward)
        time.sleep(0.3)
        return res.decision
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e
    else:
        return ""

    time.sleep(0.3)

def after_procedures():
    flush_data("episode","/tmp/episode")

if __name__ == "__main__":
    rospy.on_shutdown(after_procedures)
    n = 0
    next_action = ""

    for i in range(12):
        print n
        flush_data("particles","/tmp/p%07d" % n)
        next_action = act(next_action)
        n = n + 1
