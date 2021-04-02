import IM2GR: graph_vector_ub

@testset "upper bound" begin
  @test_throws DimensionMismatch graph_vector_ub((5, 5), 1)
  @test graph_vector_ub((3, 3, 3), 1) == 492
  @test graph_vector_ub((5, 5, 3), 2) == 5480
  @test graph_vector_ub((576, 576, 88), 1) == 751321800
end
