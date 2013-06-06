--initialization lua file

print("Running Lua Initialization script")
m = Model.New(128*3)

color = {1.0, 0.0, 0.0, 1.0}

for i = 0, 127, 1 do 
    Model.AddVertex(m, {math.cos(i / 128 * (math.pi)), math.sin(i / 128 * (math.pi)), 1}, color)
    Model.AddVertex(m, {math.cos((i+1) / 128 * (math.pi)), math.sin((i+1) / 128 * (math.pi)), 1}, color)
    Model.AddVertex(m, {0, 0, 1}, color)
    Game.AddModel(m)
end

print("Done")
