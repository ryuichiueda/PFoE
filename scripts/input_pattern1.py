#!/usr/bin/env python
import sys
import rospy
import time
import random
from t_maze_return_path.srv import ExecAction,ExecActionResponse
from std_msgs.msg import String

from pfoe.srv import EventRegist, ActionRegist, FlushData

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

def regist(action):
    rospy.wait_for_service('/pfoe/action_regist')
    try:
        sp = rospy.ServiceProxy('/pfoe/action_regist', ActionRegist)
        res = sp(action)
        return res.ok
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e
    else:
        return ""

def act(action):
    rospy.wait_for_service('/t_maze_return_path/action')
    rospy.wait_for_service('/pfoe/event_regist')

    try:
        exec_action = rospy.ServiceProxy('/t_maze_return_path/action',ExecAction)
        res = exec_action(action)

    except rospy.ServiceException, e:
        print "EXECPTION"


    try:
        sp = rospy.ServiceProxy('/pfoe/event_regist', EventRegist)
	print action, res.sensor, res.reward
        res = sp(action,res.sensor,res.reward)
        #time.sleep(0.1)
        return res.decision
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e
    else:
        return ""

def after_procedures():
    flush_data("episode","/tmp/episode")

if __name__ == "__main__":
    rospy.on_shutdown(after_procedures)

    regist("fw")
    regist("cw")
    regist("ccw")

    n = 0
    next_action = "fw"

    for i in range(10):
        print n
        flush_data("particles","/tmp/p%07d" % n)
        if n%10000 == 0:
            flush_data("episode","/tmp/episode")
        next_action = act(next_action)
        n = n + 1
