one_away_data = UInt8.(cat([0 1; 1 1], [1 2; 0 0], dims=3))
one_away_ret = [56 13.856406 11.313708]

two_away_data = UInt8.(cat([0 1 2 1 0; 1 1 1 1 1; 0 0 0 0 0], 
                           [1 2 3 4 5; 0 0 252 0 0; 1 1 1 1 1], dims=3))
two_away_ret = [654 109.352641 479834.242936]

@testset "singlethread loader" begin
  image = Image{UInt8}(one_away_data, 1)
  im2gr!(image, IM2GR.SingleThread)
  @test begin 
    length(image.ei) == one_away_ret[1] && 
    length(image.ej) == one_away_ret[1] 
  end
  @test begin
    isapprox(norm(image.evd), one_away_ret[2], atol=1e-6) && 
    isapprox(norm(image.evi), one_away_ret[3], atol=1e-6)
  end

  image = Image{UInt8}(two_away_data, 2)
  im2gr!(image, IM2GR.SingleThread)
  @test begin 
    length(image.ei) == two_away_ret[1] && 
    length(image.ej) == two_away_ret[1] 
  end
  @test begin
    isapprox(norm(image.evd), two_away_ret[2], atol=1e-6) && 
    isapprox(norm(image.evi), two_away_ret[3], atol=1e-6)
  end
end

