db = db.getSiblingDB("prayana");

// Preload tokens
db.tokens.insertMany([
    { "token": "test1" },
    { "token": "test123" },
    { "token": "hello" }
]);

// Preload registered users
db.registered_users.insertMany([
    {
        "username": "Izhaan",
        "regno": "12345",
        "token": "abc123",
        "password": "$2b$12$examplehashedpassword1234567890"  // Replace with actual bcrypt hash
    },
    {
        "username": "izzu",
        "regno": "12345",
        "token": "test123",
        "password": "$2b$12$examplehashedpassword0987654321"  // Replace with actual bcrypt hash
    },
    {
        "username": "izzu32",
        "token": "test123",
        "password": "$2b$12$g6SI1UdmxhBVz88hhVHVouDcYCrnNzF4nZ97VwEtRfkvr9cn0IGHi"
    }
]);

print("✅ Tokens and registered users added to MongoDB!");

