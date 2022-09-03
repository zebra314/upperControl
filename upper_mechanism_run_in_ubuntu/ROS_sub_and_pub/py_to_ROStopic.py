import rospy
from std_msgs.msg import String
def talker():
    pub = rospy.Publisher('chatter',String ,queue_size=10)
    rospy.init_node('talker')
    rate = rospy.Rate(1000) # 10Hz
    while not rospy.is_shutdown():
        msg = input()
        rospy.loginfo(msg)
        pub.publish(msg)
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
