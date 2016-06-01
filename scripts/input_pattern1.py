#!/usr/bin/python

import rospy
from pfoe.srv import EventRegist

def add_event(n):
    rospy.wait_for_service('/pfoe/event_regist')
    try:
        p = rospy.ServiceProxy('/pfoe/event_regist', EventRegist)
        action = "X"
        if n%3 == 0:
            sensor = "a"
        else:
            sensor = "b"
        reward = 0.0

        res = p(action,sensor,reward)
        return res.decision
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e
    else:
        return ""

if __name__ == "__main__":
    n = 0
    #while True:
    for i in range(5):
        print n
        add_event(n);
        n = n + 1
