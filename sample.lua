

stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.3, 0.3, 0.3}, 10)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)

-- ##############################################
-- the arch
-- ##############################################

-- inst = gr.node('inst')

arc = gr.node('arc')
-- inst:add_child(arc)

p1 = gr.nh_box('p1', {0, 0, 0}, 1)
arc:add_child(p1)
p1:set_material(stone)
p1:translate(-2.4, 0, -0.4)
p1:scale(0.8, 4, 0.8)

p2 = gr.nh_box('p2', {0, 0, 0}, 1)
arc:add_child(p2)
p2:set_material(stone)
p2:translate(1.6, 0, -0.4)
p2:scale(0.8, 4, 0.8)

s = gr.nh_sphere('s', {0, 0, 0}, 1)
arc:add_child(s)
s:set_material(stone)
s:scale(4, 0.6, 0.6)
s:translate(0, 4, 0)

-- #############################################
-- Let's assume that cows are spheres
-- #############################################

cow = gr.node('the_cow')
cow_poly = gr.mesh('polycow', 'Assets/cow.obj')
cow_poly:set_material(hide)

for _, spec in pairs({
			{'body', {0, 0, 0}, 1.0},
			{'head', {.9, .3, 0}, 0.6},
			{'tail', {-.94, .34, 0}, 0.2},
			{'lfleg', {.7, -.7, -.7}, 0.3},
			{'lrleg', {-.7, -.7, -.7}, 0.3},
			{'rfleg', {.7, -.7, .7}, 0.3},
			{'rrleg', {-.7, -.7, .7}, 0.3}
		     }) do
   part = gr.nh_sphere(table.unpack(spec))
   part:set_material(hide)
   cow:add_child(part)
end

-- ##############################################
-- the scene
-- ##############################################

scene = gr.node('scene')
scene:rotate('X', 23)

-- the floor

plane = gr.mesh( 'plane', 'Assets/plane.obj' )
scene:add_child(plane)
plane:set_material(grass)
plane:scale(30, 30, 30)



-- Use the instanced cow model to place some actual cows in the scene.
-- For convenience, do this in a loop.

cow_number = 1
cow_z = 0
cow_x_constant = 3
cow_y_constant = 1.3
rotationAmount = -90
polycowscale = 0.60
cowarch = gr.node("cowarch")


for _, pt in pairs({
		      {{0*cow_x_constant,cow_y_constant,cow_z}, rotationAmount},
		      {{2*cow_x_constant,cow_y_constant,cow_z}, rotationAmount},
			  {{0*cow_x_constant,cow_y_constant*2.5,cow_z}, rotationAmount},
		      {{1*cow_x_constant,cow_y_constant*2.5,cow_z}, rotationAmount},
		      {{2*cow_x_constant,cow_y_constant*2.5,cow_z}, rotationAmount}}) do
   cow_instance = gr.node('cow' .. tostring(cow_number))
   cowarch:add_child(cow_instance)
   cow_instance:add_child(cow)
   cow_instance:scale(1.4, 1.4, 1.4)
   cow_instance:rotate('Y', pt[2])
   cow_instance:translate(table.unpack(pt[1]))

   cow_number = cow_number + 1
end

cowarch:translate(-3,0,-13)

for i = 1, 6 do
   an_arc = gr.node('arc' .. tostring(i))
   an_arc:rotate('Y', (i-1) * 60)
   scene:add_child(an_arc)
   an_arc:add_child(cowarch)
end


arc1 = gr.node("arc1")
arc1:add_child(arc)
arc2 = gr.node("arc2")
arc2:add_child(arc)
arc3 = gr.node("arc3")
arc3:add_child(arc)

arc1:translate(1,0,14)
arc1:rotate("Y", 20)

arc2:translate(5,0,-11)

arc3:translate(-5.5,0,-3)
arc3:rotate("Y", -60)

scene:add_child(arc1)
scene:add_child(arc2)
scene:add_child(arc3)



buckyball = gr.mesh( 'buckyball', 'Assets/mickey.obj' )
--scene:add_child(buckyball)
buckyball:set_material(stone)
buckyball:scale(10, 10, 10)
buckyball:rotate("Y",-90)
buckyball:rotate("Z",-90)
buckyball:translate(-5,4,-1)

cow_poly:rotate("Y",-90)
cow_poly:translate(0,2.5,0)
centerpiece = gr.node("centerpiece")
centerpiece:add_child(buckyball)
centerpiece:add_child(cow_poly)
scene:add_child(centerpiece)


gr.render(scene,
	  'sample.png', 512 * 2, 512 * 2,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0})})
