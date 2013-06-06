--generate amoeba mesh

require("SimpleGeometry")

model = StartModel(precision * 3)   --TODO: right value?
Sphere(model, radius, precision)

