import IM2GR: graph_vector_ub

@testset "utils" begin
  @test_throws DimensionMismatch graph_vector_ub((5, 5), 1)
  @test graph_vector_ub((3, 3, 3), 1) == 492
  @test graph_vector_ub((5, 5, 3), 2) == 5480

  tz = zeros(Unsigned, 3, 3, 3)
  image = Image{Unsigned}(tz, 1)
  @test update_image!(image, data=tz, d=1) == false
  fz = zeros(Unsigned, 5, 5, 5)
  _d = 2
  _ub = graph_vector_ub(size(fz), _d)
  @test begin
    update_image!(image, data=fz, d=_d) == true && 
    size(image.data) == size(fz)                &&
    image.d == _d                               &&
    image.ub == _ub                             &&
    length(image.ei) == _ub
  end
end
