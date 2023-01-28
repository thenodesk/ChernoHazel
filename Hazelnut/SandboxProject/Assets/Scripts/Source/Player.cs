﻿using System;

using Hazel;

namespace Sandbox
{
    public class Player : Entity
    {
        private TransformComponent m_Transform;
        private Rigidbody2DComponent m_RigidBody;

        public float PlayerSpeed;
        public float Time = 0.0f;

        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {ID}");

            m_Transform = GetComponent<TransformComponent>();
            m_RigidBody = GetComponent<Rigidbody2DComponent>();

            PlayerSpeed = 0.25f;
        }

        void OnUpdate(float ts)
        {
            Time += ts;
            //Console.WriteLine($"Player.OnUpdate: {ts}");

            float speed = PlayerSpeed;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.W))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(KeyCode.S))
                velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.A))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(KeyCode.D))
                velocity.X = 1.0f;


            velocity *= speed;

            m_RigidBody.ApplyLinearImpulse(velocity.XY, true);

            //Vector3 translation = m_Transform.Translation;
            //translation += velocity * ts;
            //m_Transform.Translation = translation;
        }
    }
}
