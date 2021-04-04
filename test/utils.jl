import IM2GR: full_upperbound 

@testset "upper bound" begin
  @test full_upperbound((3, 3, 3), 1) == 492
  @test full_upperbound((5, 5, 3), 2) == 5480
  @test full_upperbound((576, 576, 88), 1) == 751321800
end
