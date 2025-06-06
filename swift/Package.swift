// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "DigitalOceanKit",
    platforms: [
        .macOS(.v12),
        .iOS(.v15),
        .watchOS(.v8),
        .tvOS(.v15)
    ],
    products: [
        .library(
            name: "DigitalOceanKit",
            targets: ["DigitalOceanKit"]
        ),
        .executable(
            name: "doctl-swift",
            targets: ["DigitalOceanCLI"]
        )
    ],
    dependencies: [
        .package(url: "https://github.com/apple/swift-argument-parser", from: "1.2.0"),
        .package(url: "https://github.com/apple/swift-http-types", from: "1.0.0")
    ],
    targets: [
        .target(
            name: "DigitalOceanKit",
            dependencies: [
                .product(name: "HTTPTypes", package: "swift-http-types"),
                .product(name: "HTTPTypesFoundation", package: "swift-http-types")
            ]
        ),
        .executableTarget(
            name: "DigitalOceanCLI",
            dependencies: [
                "DigitalOceanKit",
                .product(name: "ArgumentParser", package: "swift-argument-parser")
            ]
        ),
        .testTarget(
            name: "DigitalOceanKitTests",
            dependencies: ["DigitalOceanKit"]
        )
    ]
)