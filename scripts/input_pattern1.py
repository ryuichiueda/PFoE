#!/usr/bin/env python
import sys
import rospy
import time
import random
from t_maze_return_path.srv import ExecAction,ExecActionResponse
from std_msgs.msg import String

from pfoe.srv import EventRegist, FlushData

def print_episode():
    rospy.wait_for_service('/pfoe/flush_data')
    try:
        p = rospy.ServiceProxy('/pfoe/flush_data', FlushData)
        res = p("/tmp/episode","episode")
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e
    else:
        return False

    return True

def act(n):
    rospy.wait_for_service('/t_maze_return_path/action')
    rospy.wait_for_service('/pfoe/event_regist')
    try:
        exec_action = rospy.ServiceProxy('/t_maze_return_path/action',ExecAction)

        action = "fw"
        r = random.randint(0,2)
        if r == 1:
            action = "cw"
        elif r == 2:
            action = "ccw"

        res = exec_action(action)

    except rospy.ServiceException, e:
        print "EXECPTION"

 #       print action, res.result, res.sensor, res.reward

    try:
        sp = rospy.ServiceProxy('/pfoe/event_regist', EventRegist)
        res = sp(action,res.sensor,res.reward)
        time.sleep(0.3)
        return res.decision
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e
    else:
        return ""

    time.sleep(0.3)

if __name__ == "__main__":
    n = 0
    #while True:
    for i in range(100):
        print n
        next_action = act(n)
        n = n + 1
	print next_action

    print_episode()
