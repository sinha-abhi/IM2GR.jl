# order matters
data = Vector{Any}()
push!(data, UInt8.(cat([0 1; 1 1], [1 2; 0 0], dims=3)))
push!(data, UInt8.(cat([0 1 2 1 0; 1 1 1 1 1; 0 0 0 0 0], 
                       [1 2 3 4 5; 0 0 252 0 0; 1 1 1 1 1], dims=3)))
push!(data, UInt8.(cat([0 1 2 1 0; 1 1 1 1 1; 0 0 0 0 0], 
                       [1 2 3 4 5; 0 0 252 0 0; 1 1 1 1 1], dims=3)))
d = [1 1 2]
ret = [
  [56 13.856406 11.313708];
  [334 36.083237 367031.792222];
  [654 109.352641 479834.242936]
]

function run_loader_tests(mode::ConstructionMode, data, d, ret)
  for t = 1 : length(data)
    image = Image{UInt8}(data[t], d[t])
    im2gr!(image, mode)
    # @printf("expected: %d, got %d\n", Int(ret[t, 1]), length(image.ei))
    @test begin 
      length(image.ei) == Int(ret[t, 1]) && 
      length(image.ej) == Int(ret[t, 1])
    end
    @test begin
      isapprox(norm(image.evd), ret[t, 2], atol=1e-6) && 
      isapprox(norm(image.evi), ret[t, 3], atol=1e-6)
    end
  end

  nothing
end

@testset "singlethread" begin
  run_loader_tests(IM2GR.SingleThread, data, d, ret)
end

@testset "multithread" begin
  run_loader_tests(IM2GR.MultiThread, data, d, ret)
end

