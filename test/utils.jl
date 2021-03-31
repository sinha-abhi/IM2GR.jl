import IM2GR: graph_vector_ub

@testset "upper bound" begin
  @test_throws DimensionMismatch graph_vector_ub((5, 5), 1)
  @test graph_vector_ub((3, 3, 3), 1) == 492
  @test graph_vector_ub((5, 5, 3), 2) == 5480
  @test graph_vector_ub((576, 576, 88), 1) == 751321800
end

@testset "update" begin
  tz = zeros(Unsigned, 3, 3, 3)
  image = Image{Unsigned}(tz, 1)
  @test update_image!(image, data=tz, d=1) == false
  fz = zeros(Unsigned, 5, 5, 5)
  @test begin
    update_image!(image, data=fz) == true && 
    size(image.data) == size(fz)
  end

  _d = 2
  _ub = graph_vector_ub(size(fz), _d)
  @test begin
    update_image!(image, d=_d) == true && 
    image.d == _d                      &&
    image.ub == _ub                    &&
    length(image.ei) == _ub
  end
end
