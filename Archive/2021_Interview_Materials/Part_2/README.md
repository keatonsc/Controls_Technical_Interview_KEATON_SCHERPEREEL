# Exo Controller - Part 2 (4 hours)

Your team is designing an ExoBoot controller to augment someone while they’re walking. Given the data from the sensors on the exo, which include a 6-axis IMU located on the calf, an encoder located on the ankle, a motor encoder, and a current and voltage reading from the motor, the goal is to determine what commands to send to the motor. Hopefully, those commands will result in a helpful amount of torque when a user is walking, but will not hinder the user in any way when the user is doing other motions.

One of the controls engineers on your team has figured out a way to send the appropriate motor commands given a desired ankle torque, so now all you have to do is determine how much torque to output at the ankle at any given point in time (keep in mind, the exo can only apply positive ankle torques, which move the ankle joint in the plantarflexion direction, and a zero ankle torque would allow the ankle joint to move freely). Thankfully, another controls engineer on the team found a useful dataset online that reports ideal torque at the ankle joint of an ankle exo very similar to the exoboot and includes data from different users and movements.

With all this information you start brainstorming ways to recreate those ankle torques on the ExoBoot and you have a lot of questions: Based on this dataset, how might we calculate how much ankle torque our user's prefer during each part of their step? Can we use gait phase (when the foot is on the ground vs when it isn’t) to understand how/when to go from zero torque to peak torque? Can we predict when users are going uphill and use that to understand how/when/what the peak torque should be? Can we detect when a user is not walking so that we make sure the exo doesn’t hinder those movements?

You know you can’t prototype solutions for all your questions, but want to see if you can address one of them and show your team a quick proof of concept so you can decide how to move forward with the project.

**Side Note**: In your attempts to make a proof of concept, you enlist a junior software engineer to make some quick helper functions for you. You get the helper functions back from them and notice that they’re functional, but could be implemented better. You decide to use this as a teaching moment and provide a code review for your coworker.

-------------------------

## Candidate Action Items:

1. Given the provided datasets and the file of helper functions, create a basic implementation for 1 of the following functions:
	- predictAnkleTorque
	- detectWalking
	- isUserInStance
	- calculateGroundSlope
	Keep in mind that the ultimate goal would be to try your implementation out in real time, on a real device, on a real human.

2. Review the file of helper functions. Within the file, implement changes that would improve the quality of the file.

3. Please fill out the following:

	- Did you implement Task 1 in Matlab or Python?
		- >ADD YOUR ANSWER HERE
	- Which function did you choose to implement for Task 1?
		- >ADD YOUR ANSWER HERE
	- Briefly describe your implementation (1-3 sentences).
		- >ADD YOUR ANSWER HERE
	- Briefly describe your improvements to the file of helper functions.

-------------------------

## Rules of Engagement

0. We don't expect you to spend more than 4 hours working on this technical challenge. Do what you can in that time. If you want to work more feel free, but please push your final solution and create your pull request within 24 hours of receiving this task, as that is when we will evaluate your solution.
1. You may implement your solution for Task 1 in either Matlab or Python.
2. If you choose to implement in Python, you may only use the following non-standard libraries:
	- numpy
	- matplotlib
		(if matplotlib doesn't work because of a kiwisolver error, try uninstalling and reinstalling kiwisolver)
	- scipy
	- if necessary you can use simple standard libaries like (os, sys, math, copy, etc)
3. If you choose to implement in Matlab, DO NOT use any functions from non-standard toolboxes or add-ons.
4. Which function you choose to implement will dictate which folder of the repo you work in. However, each folder contains the following files/folders:
	- dataset: a folder containing csv files of data collected from people using the exo. You can use this folder to make/self-test your implementaion. Note: The dataset we use to evaluate your implementation may contain a few additional files that you do not have access too.
	- functionName: this is the file where you will implement your function
	- exampleFunctionNameFunction: provides an example of what the output of your function
	  implementation may look like
	- testFunctionNameFunction: provides a way for you to test your function implementation on the files in the dataset folder
	- helperFunctions: technically this file lives outside of the folder you'll be working in, but it contains a number of functions that may make Task 1 a little easier. Use           them if you so please.
5. If you create additional code to help you analyze data or otherwise help you implement your function, put them in 1 or more additional files within the function's folder. Don't forget; we are interested in both your solution and your thought process.

-------------------------

## Some Biomechanics Notes

1. There are multiple ways to solve these tasks and you DO NOT need any biomechanics background to solve any of them, so if that's not you're background or approach, that's okay. In case it's useful, we've defined a couple of terms below:
	- Heelstrike: when your heel hits the ground at the beginning of a step
	- Stance: the phase of the step in which your foot is on the ground
	- Swing: the phase of the step in which your foot is in the air
	- Step: the movement from heelstrike to heelstrike on the SAME foot

2. For those WITH biomechanics background, please take note of the following, because our conventions  are likely different than what you learned:
	- A single step starts at heelstrike and ends when that SAME foot heelstrikes again
	- Positive ankle velocities correspond to plantarflexion. Negative ankle velocities correspond
	  to dorsiflexion.

-------------------------

## A Few More Useful Details

The datasets provided are different for each problem and include a variety of files that correspond to different users completing different movements, such as walking, running, going upstairs, and going downstairs. Each csv in the dataset, sampled at 100 Hz,  lists a variety of variables, including but not limited to:

Name          | Description           | Units | Notes
-----------------------|-----------------------|-------|-------------------
accelx, accely, accelz | acceleration from IMU | bits  | 2048 bits/(m/s^2)
gyrox, gyroy, gyroz    | angular velocity from IMU | bits | 32.768 bits/(deg/s)
ank_ang                | position from ankle encoder | 100 x degrees	 | This is a relative measure of the angle between your calf and your foot.
ank_vel                | ankle angular velocity estimate calculated from ank_ang | 10 x (degrees/s) | Positive values correspond to plantarflexion. Negative values correspond to dorsiflexion.
movement               | Describes what the user is currently doing| N/A | 0 = unknown, 1 = walking, 2 = running, 3 = upstairs, 4 = downstairs
gait_state             | Describes whether the user is in stance or swing | N/A | <0 = unknown, 0 = swing, 1 = stance. At heelstrike (i.e. when a user begin's a new step, gait_state will typically go from 0 to 1)
ank_torque             | How much ankle torque the user is receiving| mNm
state_time             | System time           | ms    |
step_time              | Time since the last heel strike on this leg (i.e. time since the step began)| ms |
speed                  | Rough estimate of a user’s ground speed | 100 x (miles/hour) |
incline                | Rough estimate of a the ground slope | % grade | Positive values correspond to a positive slope (e.g. uphill). Negative values correspond to a negative slope (e.g. downhill).

**Note**: The IMU is located on the armature of the ExoBoot and the ankle encoder is located at the ankle joint.
See the ExoBoot image for details.

![Dephy Exo Boot](/Part_2/exoBoot.jpg)
