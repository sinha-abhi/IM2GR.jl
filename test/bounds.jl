import IM2GR: fullbound 

@testset "fullbound" begin
  @test fullbound((3, 3, 3), 1) == 492
  @test fullbound((5, 5, 3), 2) == 5480
  @test fullbound((576, 576, 88), 1) == 751321800
end
