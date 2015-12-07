-- puppet.lua
-- A simplified puppet without posable joints, but that
-- looks roughly humanoid.

rootnode = gr.node('root')
rootnode:rotate("y",0)
rootnode:translate(0.0, 0.0, -15.0)

eyeRadius = 0.35;
pupilDistance = 0.31;

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

skinMat = gr.material({248/255, 194/255, 158/255}, {0.0, 0.0, 0.0}, 100)
hairMat = gr.material({133/255, 74/255, 16/255}, {0, 0, 0}, 10)
shirtMat = gr.material({215/255, 212/255, 143/255}, {0, 0, 0}, 10)
pantsMat = gr.material({40/255, 58/255, 94/255}, {0.1, 0.1, 0.1}, 100)

nr = {0,0,0}

torsoJoint = gr.joint("torseJoint",nr,{-180,0,180});

rootnode:add_child(torsoJoint);

torso = gr.mesh('cube', 'torso')
torsoJoint:add_child(torso)
torso:set_material(red)
torso:scale(1.2,2.5,1);
torso:translate(0,-0.25,0)
torso:set_material(shirtMat)

chestJoint = gr.joint("chestJoint", {-60,0,60}, {-60,0,60})
torsoJoint:add_child(chestJoint)
chestJoint:translate(0,0.5,0)

chest = gr.mesh('cube', 'chest')
chestJoint:add_child(chest)
chest:scale(1.2,1,1.2)
chest:translate(0,0.7,0)
chest:set_material(shirtMat)

neckJoint = gr.joint("neckJoint",{-60,0,30},nr)
chestJoint:add_child(neckJoint)
neck = gr.mesh("cylinder","neck")
neckJoint:add_child(neck)
neckJoint:translate(0,1,0)
neck:scale(0.4,0.2,0.4)
neck:translate(0,0.4,0)
neck:set_material(skinMat)

headJoint = gr.joint("headJoint",nr,{-150,0,150})
headJoint:head(1)
neckJoint:add_child(headJoint)
headJoint:translate(0,0.2,0)
head = gr.mesh("sphere","head")
headJoint:add_child(head)
head:translate(0,1,0)
head:set_material(skinMat)

hair = gr.mesh("sphere","hair")
headJoint:add_child(hair)
hair:translate(0,1.8,0.4)
hair:scale(1.2,0.8,1.2)

hair:rotate("x",-30)
hair:set_material(hairMat)

leftEyeJoint = gr.joint("leftEyeJoint",{-90,0,90},{-90,0,90})
headJoint:add_child(leftEyeJoint)
leftEyeJoint:translate(0.3,0.8,0.8)

leftEye = gr.mesh("sphere","leftEye")
leftEyeJoint:add_child(leftEye)
leftEye:scale(eyeRadius,eyeRadius,eyeRadius)
leftEye:set_material(white)

leftPupil = gr.mesh("sphere","leftPupil")
leftEyeJoint:add_child(leftPupil)
leftPupil:scale(0.05,0.05,0.05)
leftPupil:translate(0,0,pupilDistance)

rightEyeJoint = gr.joint("rightEyeJoint",{-90,0,90},{-90,0,90})
headJoint:add_child(rightEyeJoint)
rightEyeJoint:translate(-0.3,0.8,0.8)

rightEye = gr.mesh("sphere","rightEye")
rightEyeJoint:add_child(rightEye)
rightEye:scale(eyeRadius,eyeRadius,eyeRadius)
rightEye:set_material(white)

rightPupil = gr.mesh("sphere","rightPupil")
rightEyeJoint:add_child(rightPupil)
rightPupil:scale(0.05,0.05,0.05)
rightPupil:translate(0,0,pupilDistance)



nose = gr.mesh("sphere","nose")
headJoint:add_child(nose)
nose:scale(0.1,0.1,0.1)
nose:translate(0,0.4,0.8)
nose:set_material(skinMat)

leftEar = gr.mesh("sphere","leftEar")
headJoint:add_child(leftEar)
leftEar:scale(0.2,0.2,0.1)
leftEar:translate(0.9,0.7,0)
leftEar:set_material(skinMat)

rightEar = gr.mesh("sphere","rightEar")
headJoint:add_child(rightEar)
rightEar:scale(0.2,0.2,0.1)
rightEar:translate(-0.9,0.7,0)
rightEar:set_material(skinMat)

leftArmJoint = gr.joint("leftArmJoint",{-80,0,200},{-100,0,80})
chestJoint:add_child(leftArmJoint)
leftArmJoint:rotate("y",-90)
leftSleeve = gr.mesh('cylinder',"leftSleeve")
leftArmJoint:add_child(leftSleeve)
leftSleeve:scale(0.25,0.75,0.25)
leftArmJoint:translate(0.8,0.7,0)
leftArmJoint:rotate('z',15)
leftSleeve:translate(0,-0.5,0)
leftSleeve:set_material(shirtMat)

leftArm = gr.mesh("cylinder","leftArm")
leftArmJoint:add_child(leftArm)
leftArm:translate(0,-2,0)
leftArm:scale(0.20,0.7,0.20)
leftArm:set_material(skinMat)

leftElbow = gr.joint("leftElbow",{0,0,250},nr)
leftElbow:rotate("y",-90)
leftArmJoint:add_child(leftElbow)
leftElbow:translate(0,-2,0)
leftForearm = gr.mesh("cylinder","leftForearm")
leftElbow:add_child(leftForearm)
leftForearm:scale(0.18,0.7,0.18)
leftForearm:translate(0,-0.6,0)
leftForearm:set_material(skinMat)

leftWrist = gr.joint("leftWrist",{-10,0,50},{-90,0,30})
leftElbow:add_child(leftWrist)
leftWrist:translate(0,-1.2,0)

leftHand = gr.mesh("sphere","leftHand")
leftWrist:add_child(leftHand)
leftHand:scale(0.3,0.5,0.2)
leftHand:translate(0,-0.5,0)
leftHand:set_material(skinMat)

rightArmJoint = gr.joint("rightArmJoint",{-80,0,200},{-100,0,80})
chestJoint:add_child(rightArmJoint)
rightArmJoint:rotate("y",90)

rightSleeve = gr.mesh('cylinder',"rightSleeve")
rightArmJoint:add_child(rightSleeve)
rightSleeve:scale(0.25,0.75,0.25)
rightArmJoint:translate(-0.8,0.7,0)
rightArmJoint:rotate('z',-15)
rightSleeve:translate(0,-0.5,0)
rightSleeve:set_material(shirtMat)

rightArm = gr.mesh("cylinder","rightArm")
rightArmJoint:add_child(rightArm)
rightArm:translate(0,-2,0)
rightArm:scale(0.20,0.7,0.20)
rightArm:set_material(skinMat)

rightElbow = gr.joint("rightElbow",{0,0,250},nr)
rightElbow:rotate("y",90)
rightArmJoint:add_child(rightElbow)
rightElbow:translate(0,-2,0)
rightForearm = gr.mesh("cylinder","rightForearm")
rightElbow:add_child(rightForearm)
rightForearm:scale(0.18,0.7,0.18)
rightForearm:translate(0,-0.6,0)
rightForearm:set_material(skinMat)

rightWrist = gr.joint("rightWrist",{-10,0,50},{-90,0,30})
rightElbow:add_child(rightWrist)
rightWrist:translate(0,-1.2,0)

rightHand = gr.mesh("sphere","rightHand")
rightWrist:add_child(rightHand)
rightHand:scale(0.3,0.5,0.2)
rightHand:translate(0,-0.5,0)
rightHand:set_material(skinMat)


hipJoint = gr.joint("hipJoint",{-80,0,80},{-80,0,80})
torsoJoint:add_child(hipJoint)
hipJoint:translate(0,-1.4,0)

hip = gr.mesh("cube","hip")
hipJoint:add_child(hip)
hip:scale(0.95,0.25,0.95)
hip:translate(0,-0.2,0)
hip:set_material(pantsMat)

leftLegJoint = gr.joint("leftLegJoint",{-90,0,90},{-90,0,90})
hipJoint:add_child(leftLegJoint)
leftThigh = gr.mesh("cylinder","leftThigh")
leftLegJoint:add_child(leftThigh)
leftLegJoint:translate(0.3,-0.2,0)
leftThigh:translate(0,-1,0)
leftThigh:scale(0.2,1,0.2)
leftThigh:set_material(pantsMat)

leftKnee = gr.joint("leftKnee",{0,0,180},nr)
leftLegJoint:add_child(leftKnee)
leftKnee:translate(0,-1.9,0)

leftCrus = gr.mesh("cylinder","leftCrus")
leftKnee:add_child(leftCrus)
leftCrus:translate(0,-1,0)
leftCrus:scale(0.2,1,0.2)
leftCrus:set_material(pantsMat)

leftAnkleJoint = gr.joint("leftAnkleJoint",{-30,0,60},{-60,0,60})
leftKnee:add_child(leftAnkleJoint)
leftAnkleJoint:translate(0,-2,0)
leftFoot = gr.mesh("sphere","leftFoot")
leftAnkleJoint:add_child(leftFoot)

leftFoot:scale(0.2,0.2,0.6)
leftFoot:translate(0.0,0,0.5)
leftFoot:set_material(white)

rightLegJoint = gr.joint("rightLegJoint",{-90,0,90},{-90,0,90})
hipJoint:add_child(rightLegJoint)
rightThigh = gr.mesh("cylinder","rightThigh")
rightLegJoint:add_child(rightThigh)
rightLegJoint:translate(-0.3,-0.2,0)
rightThigh:translate(0,-1,0)
rightThigh:scale(0.2,1,0.2)
rightThigh:set_material(pantsMat)

rightKnee = gr.joint("rightKnee",{0,0,180},nr)
rightLegJoint:add_child(rightKnee)
rightKnee:translate(0,-1.9,0)

rightCrus = gr.mesh("cylinder","rightCrus")
rightKnee:add_child(rightCrus)
rightCrus:translate(0,-1,0)
rightCrus:scale(0.2,1,0.2)
rightCrus:set_material(pantsMat)

rightAnkleJoint = gr.joint("rightAnkleJoint",{-30,0,60},{-60,0,60})
rightKnee:add_child(rightAnkleJoint)
rightAnkleJoint:translate(0,-2,0)
rightFoot = gr.mesh("sphere","rightFoot")
rightAnkleJoint:add_child(rightFoot)

rightFoot:scale(0.2,0.2,0.6)
rightFoot:translate(0.0,0,0.5)
rightFoot:set_material(white)

return rootnode
