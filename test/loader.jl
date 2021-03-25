@testset "singlethread loader" begin
  data = UInt8.(cat([0 1; 1 1], [1 2; 0 0], dims=3))
  image = Image{UInt8}(data, 1)
  im2gr!(image, IM2GR.SingleThread, track=false)
  @test length(image.ei) == 56 && length(image.ej) == 56
  @test begin
    isapprox(norm(image.evd), 13.856406, atol=1e-6) && 
    isapprox(norm(image.evi), 11.313708, atol=1e-6)
  end

  data = UInt8.(cat([0 1 2 1 0; 1 1 1 1 1; 0 0 0 0 0], [1 2 3 4 5; 0 0 252 0 0; 1 1 1 1 1], dims=3))
  image = Image{UInt8}(data, 2)
  im2gr!(image, IM2GR.SingleThread, track=false)
  @test length(image.ei) == 654 && length(image.ej) == 654
  @test begin
    isapprox(norm(image.evd), 109.352641, atol=1e-6) && 
    isapprox(norm(image.evi), 479834.242936, atol=1e-6)
  end
end
